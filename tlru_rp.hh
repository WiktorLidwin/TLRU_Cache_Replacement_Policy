 #ifndef __MEM_CACHE_REPLACEMENT_POLICIES_TLRU_RP_HH__
 #define __MEM_CACHE_REPLACEMENT_POLICIES_TLRU_RP_HH__
  
 #include "mem/cache/replacement_policies/base.hh"
 #include <chrono>
 namespace gem5
 {
  
 struct TLRURPParams;
  
 namespace replacement_policy
 {
  
 class TLRU : public Base
 {
   protected:
     struct TLRUReplData : ReplacementData
     {
        unsigned refCount;
        Tick lastTouchTick;
        TLRUReplData() : lastTouchTick(0), refCount(0) {}
        
     };
  
   public:
     typedef TLRURPParams Params;
     TLRU(const Params &p);
     ~TLRU() = default;
  
     void invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
                                                                     override;
  
     void touch(const std::shared_ptr<ReplacementData>& replacement_data) const
                                                                      override;
  
     void reset(const std::shared_ptr<ReplacementData>& replacement_data) const
                                                                      override;
  
     ReplaceableEntry* getVictim(const ReplacementCandidates& candidates) const
                                                                      override;
  
     std::shared_ptr<ReplacementData> instantiateEntry() override;
 };
   } // namespace replacement_policy
 } // namespace gem5
  
 #endif // __MEM_CACHE_REPLACEMENT_POLICIES_TLRU_RP_HH__ 
