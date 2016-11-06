// Borrowed from Message.h

namespace supp {

   //! Expanded timestamp for 250 MHz * 14 bit epochs
   uint64_t FullTimeStamp(uint32_t epoch, uint16_t stamp)
   { return ((uint64_t) epoch << 14) | (stamp & 0x3fff); }

   //! Expanded timestamp for 250/8*5 MHz * 19 bit epochs
   uint64_t FullTimeStamp2(uint32_t epoch, uint32_t stamp)
   { return ((uint64_t) epoch << 19) | (stamp & 0x7ffff); }

}
