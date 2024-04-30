  
 #include "mem/cache/replacement_policies/tlru_rp.hh"
  
 #include <cassert>
 #include <memory>
  
 #include "params/TLRURP.hh"
 #include "sim/cur_tick.hh"
  
 namespace gem5
 {
  
 GEM5_DEPRECATED_NAMESPACE(ReplacementPolicy, replacement_policy);
 namespace replacement_policy
 {
  
 TLRU::TLRU(const Params &p)
   : Base(p)
 {
 }
  
 void
 TLRU::invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
 {
     // Reset last touch timestamp
    std::static_pointer_cast<TLRUReplData>(
         replacement_data)->lastTouchTick = Tick(0); 

    std::static_pointer_cast<TLRUReplData>(replacement_data)->refCount = 0;
 }
  
 void
 TLRU::touch(const std::shared_ptr<ReplacementData>& replacement_data) const
 {
     // Update last touch timestamp
     std::static_pointer_cast<TLRUReplData>(
         replacement_data)->refCount++;

     std::static_pointer_cast<TLRUReplData>(
         replacement_data)->lastTouchTick = curTick();
 }
  
 void
 TLRU::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
 {
     // Set last touch timestamp
     std::static_pointer_cast<TLRUReplData>(
         replacement_data)->refCount = 1;

     std::static_pointer_cast<TLRUReplData>(
         replacement_data)->lastTouchTick = curTick();
 }
  
 ReplaceableEntry*
 TLRU::getVictim(const ReplacementCandidates& candidates) const
 {
     // There must be at least one replacement candidate
     assert(candidates.size() > 0);
  
     // Visit all candidates to find victim
     ReplaceableEntry* victim = candidates[0];
     bool found = false;
     for (const auto& candidate : candidates) {
         // Update victim entry if necessary
         if (std::static_pointer_cast<TLRUReplData>(
                     candidate->replacementData)->lastTouchTick <
                 std::static_pointer_cast<TLRUReplData>(
                     victim->replacementData)->lastTouchTick
                     && std::static_pointer_cast<TLRUReplData>(
                     candidate->replacementData)->refCount <
                 std::static_pointer_cast<TLRUReplData>(
                     victim->replacementData)->refCount
                     ) {
             found = true;
             victim = candidate;
         }
     }
     if (found)
      return victim;

    for (const auto& candidate : candidates) {
         // Update victim entry if necessary
         if (std::static_pointer_cast<TLRUReplData>(
                     candidate->replacementData)->lastTouchTick <
                 std::static_pointer_cast<TLRUReplData>(
                     victim->replacementData)->lastTouchTick
                     ) {
             
             victim = candidate;
         }
     }
     return victim;

 }
  
 std::shared_ptr<ReplacementData>
 TLRU::instantiateEntry()
 {
     return std::shared_ptr<ReplacementData>(new TLRUReplData());
 }
  
 } // namespace replacement_policy
 } // namespace gem5