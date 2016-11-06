#include "LmdFile.h"

#include <stdio.h>
#include <map>
#include <utility>

#include "TFile.h"
#include "TString.h"

#include "Support.h"
#include "analysisHistograms.h"
#include "analysisParameters.h"

#define DEBUGMODE

// buffer for hits
// pair full time - channel:adc
std::multimap< uint64_t, std::pair<uint8_t, uint16_t> > fTimeAdcMap;

// buffer for aux messages
// pair full time - aux channel
std::multimap< uint64_t, uint8_t > fTimeAuxMap;

void CheckAndBuildEvent(analysisHistograms& histosObj, triggerWindow_t trigWins);

int main(int numc, char* args[])
{
   const char* fname = "test.lmd";
   if (numc>1) fname = args[1];
   //printf("Try to open lmd file %s\n", fname);
   mbs::LmdFile f;
   if (!f.OpenRead(fname)) {
      printf("Cannot open file for reading - error %d\n", f.LastError());
      return 1;
   }
   printf ("Imported %s\n", fname);

   TString suffix("");
   if (numc>2) {
      suffix = TString(args[2]);
   }

   // Prepare output ROOT file and histograms
   TString rootFileName("res.root");
   analysisHistograms histosObj;
   histosObj.SetOutputRootFileName(rootFileName);
   histosObj.SetSuffix(suffix);

   // Import trigger windows
   triggerWindow_t trigWins(874, 1026); //    trigWins(874, 1026);
   TString inputFileBasename("");
   TString trigWinCfgFilename("TriggerWindows.cfg");
   if (numc>3) {
      analysisParameters trigWinsMap(trigWinCfgFilename);
      inputFileBasename = TString(args[3]);
      trigWins = trigWinsMap.GetWindow(inputFileBasename);
      printf ("Imported trigger window: [%ld - %ld] from %s\n", trigWins.first, trigWins.second, trigWinCfgFilename.Data());
   }

   // Declarations
   uint32_t currentEpoch = 0;
   uint64_t hitFullTime = 0;
   std::pair<uint8_t, uint16_t> curChAdcPair;

   mbs::EventHeader* hdr = 0;
   mbs::SubeventHeader* subEventHdr = 0;

   unsigned int eventCounter = 0;
   unsigned int subEventCounter = 0;

   uint16_t* subEvData = 0;

   // Loop over events in the LMD file
   while ((hdr = f.ReadEvent()) != 0)
   {
#ifdef DEBUGMODE
      printf("%d: Event %u length %u num %u\n", eventCounter, hdr->EventNumber(), hdr->SubEventsSize(), hdr->NumSubevents());
#endif

      subEventHdr = 0;
      subEventCounter = 0;

      // Loop over subevents in the current event
      while ((subEventHdr = hdr->NextSubEvent(subEventHdr)) != 0)
      {
         uint32_t subEvNwords = subEventHdr->iWords; // subsubevent<(subEvNwords-8)/3

#ifdef DEBUGMODE
         printf ("\t%d: Subevent\tiWords=%d\tRawSize=% 4d\tRawSize/6=% 4d\n", subEventCounter, subEvNwords, subEventHdr->RawDataSize(), subEventHdr->RawDataSize()/6);
#endif
         subEvData = (uint16_t*)(subEventHdr->RawData());

         for (uint32_t subsubevent=0; subsubevent<subEventHdr->RawDataSize()/6-2; subsubevent++)
         {
            uint8_t type; // 3 bits used
            uint8_t roc; // 3 bits used
            uint8_t nx; // 2 bits used
            uint8_t ltsmsb; // 3 bits used
            uint16_t timestamp; // 14 bits
            uint8_t ch; // 7 bits used
            uint16_t adc; // 12 bits used
            uint32_t epoch; // 32 bits used
            uint32_t evnum; // 24 bits used
            uint8_t flag; // 2 bits used

            uint32_t bufferCursor = subsubevent*3;
            type = subEvData[bufferCursor+0] & 0x7; // 3 bits

            //printf ("\t\t");
            switch (type) {
            case 0:
               //printf ("NOP "); printf ("%04x%04x%04x - ", subEvData[bufferCursor+2], subEvData[bufferCursor+1], subEvData[bufferCursor+0]);
               //printf ("\n");
               break;
            case 1:
               //printf ("HIT "); printf ("%04x%04x%04x - ", subEvData[bufferCursor+2], subEvData[bufferCursor+1], subEvData[bufferCursor+0]);
               roc = (subEvData[bufferCursor+0] >> 3) & 0x7; // 3 bits
               nx = (subEvData[bufferCursor+0] >> 6) & 0x3; // 2 bits
               ltsmsb = (subEvData[bufferCursor+0] >> 8) & 0x7; // 3 bits
               timestamp = (((subEvData[bufferCursor+1] & 0x01FF) << 5) & 0x3FE0) | ((subEvData[bufferCursor+0] >> 11) & 0x001F);
               ch = (subEvData[bufferCursor+1] >> 9) & 0x007F; // 7 bits
               adc = (subEvData[bufferCursor+2] >> 1) & 0x0FFF; // 12 bits

               // Take into account 'last epoch' flag
               if (((subEvData[bufferCursor+2] >> 15) & 0x1) == 1)
                  hitFullTime = supp::FullTimeStamp(currentEpoch-1, timestamp);
               else
                  hitFullTime = supp::FullTimeStamp(currentEpoch, timestamp);
#ifdef DEBUGMODE
               // FullTime:%ld - hitFullTime
               printf("Msg:1 Roc:%d Nx:%d Chn:%02x Ts:%04x Last:%d Msb:%d Adc:%03x Pup:%d Oflw:%d\n",
                                       roc, nx, ch, timestamp,
                                       (subEvData[bufferCursor+2] >> 15) & 0x1,  // 1 bit last epoch
                                       ltsmsb, adc,
                                       (subEvData[bufferCursor+2] >> 13) & 0x1,   // 1 bit pile-up
                                       (subEvData[bufferCursor+2] >> 14) & 0x1   // 1 bit overflow
                                       );
#endif
               curChAdcPair = std::make_pair(ch, adc);
               fTimeAdcMap.insert(std::pair<uint32_t, std::pair<uint8_t, uint16_t> >(hitFullTime, curChAdcPair));
               CheckAndBuildEvent(histosObj, trigWins);
               break;
            case 2:
               //printf ("EPO "); printf ("%04x%04x%04x - ", subEvData[bufferCursor+2], subEvData[bufferCursor+1], subEvData[bufferCursor+0]);
               roc = (subEvData[bufferCursor+0] >> 3) & 0x7; // 3 bits
               epoch = ((subEvData[bufferCursor+2] & 0x00FF) << 24) | (subEvData[bufferCursor+1] << 8) | ((subEvData[bufferCursor+0] >> 8) & 0x00FF);
#ifdef DEBUGMODE
               printf("Msg:2 Roc:%d Epoch:%x Missed:00\n", roc, epoch);
#endif
               // Save epoch and use it later in hit/aux unpacking for full time determination
               currentEpoch = epoch;
               break;
            case 3:
               //printf ("SYN "); printf ("%04x%04x%04x - ", subEvData[bufferCursor+2], subEvData[bufferCursor+1], subEvData[bufferCursor+0]);
               roc = (subEvData[bufferCursor+0] >> 3) & 0x7; // 3 bits
               ch =  (subEvData[bufferCursor+0] >> 6) & 0x3; // 2 bits
//               timestamp = (((subEvData[bufferCursor+1] /*& 0x3F*/) << 8) & 0x3F00 ) | ((subEvData[bufferCursor+0] >> 8) & 0xFF); //TODO check and fix
               timestamp = (((subEvData[bufferCursor+1] /*& 0x3F*/) << 9) & 0x3E00 ) | ((subEvData[bufferCursor+0] >> 7) & 0x01FF);
               evnum = ((subEvData[bufferCursor+2] & 0x3FFF) << 10) | ((subEvData[bufferCursor+1] >> 6) & 0x03FF);
               flag = (subEvData[bufferCursor+2] >> 14) & 0x3;
#ifdef DEBUGMODE
               printf("Msg:3 Roc:%d SyncChn:%d EpochLSB:0 Ts:%04x Data:%x Flag:%x\n", roc, ch, timestamp, evnum, flag);
#endif
               break;
            case 4:
               //printf ("AUX "); printf ("%04x%04x%04x - ", subEvData[bufferCursor+2], subEvData[bufferCursor+1], subEvData[bufferCursor+0]);
               roc = (subEvData[bufferCursor+0] >> 3) & 0x7; // 3 bits
               ch =  (subEvData[bufferCursor+0] >> 6) & 0x7F; // 7 bits
//               timestamp = (((subEvData[bufferCursor+1] << 3) & 0x3FF8) | ((subEvData[bufferCursor+0] >> 13) & 0x7)) & 0x03FFF; //TODO check and fix
               timestamp = (((subEvData[bufferCursor+1] << 4) & 0x3FF0) | ((subEvData[bufferCursor+0] >> 12) & 0xF)) & 0x03FFF;

               // Take into account 'epoch lsb'
               if (((subEvData[bufferCursor+1] >> 10) & 0x1) == (currentEpoch & 0x1))
                  hitFullTime = supp::FullTimeStamp(currentEpoch, timestamp);
               else
                  hitFullTime = supp::FullTimeStamp(currentEpoch-1, timestamp);

               fTimeAuxMap.insert(std::pair<uint32_t, uint8_t>(hitFullTime, ch));

#ifdef DEBUGMODE
               // FullTime:%ld hitFullTime
               printf("Msg:4 Roc:%d AuxChn:%02d EpochLSB:%d Ts:%04x Falling:%d Overflow:%d\n", roc, ch, (subEvData[bufferCursor+1] >> 10) & 0x1, timestamp, 0, 0);
#endif
               break;
            case 7:
               //printf ("SYS "); printf ("%04x%04x%04x - ", subEvData[bufferCursor+2], subEvData[bufferCursor+1], subEvData[bufferCursor+0]);
               //printf ("\n");
               break;
            default:
               //printf ("--- "); printf ("%04x%04x%04x - ", subEvData[bufferCursor+2], subEvData[bufferCursor+1], subEvData[bufferCursor+0]);
               //printf ("\n");
               break;
            }
         }
         //printf("\n");
         subEventCounter++;
      }

      eventCounter++;
      if (eventCounter > 100000) break;
   }

   // Write histograms into the output ROOT file and close it
   histosObj.Write();

   return 0;
}

void CheckAndBuildEvent(analysisHistograms& histosObj, triggerWindow_t trigWins)
{
   //TODO set max size here
   if (fTimeAuxMap.size() > 100) {
      // The buffer of aux messages is full enough to build events
#ifdef DEBUGMODE
      printf ("\nStarting building event.\n");
#endif

      // Declare iterators
      std::multimap< uint64_t, std::pair<uint8_t, uint16_t> >::iterator timeAdcMap_iter;
      std::multimap< uint64_t, uint8_t >::iterator timeAuxMap_iter;

      uint64_t currAuxTimestamp=0;
      uint64_t leftTrigWin=0;
      uint64_t rightTrigWin=0;
      uint64_t currHitTimestamp=0;
      uint8_t currChannel=0;
      uint16_t currAdc=0;
      int counterOfErasedAux=0;
      int counterOfErasedHits=0;
      int counterOfHitsBelowEventWin=0;
      int counterOfHitsInEvent=0;

      std::pair<uint8_t, uint16_t> curChAdcPair;

      // Loop over triggers in the buffer
      for (timeAuxMap_iter = fTimeAuxMap.begin(); timeAuxMap_iter != fTimeAuxMap.end(); ++timeAuxMap_iter)
      {
         // Extract trigger timestamp
         currAuxTimestamp = (*timeAuxMap_iter).first;
         // Set the time window for the event
         leftTrigWin = currAuxTimestamp + trigWins.first;
         rightTrigWin = currAuxTimestamp + trigWins.second;

         // Initialize counters
         counterOfHitsInEvent=0;
         counterOfHitsBelowEventWin=0;

         // Loop over hits in order to find those in the defined window
         for (timeAdcMap_iter = fTimeAdcMap.begin(); timeAdcMap_iter != fTimeAdcMap.end(); ++timeAdcMap_iter) {

            // Extract hit timestamp
            currHitTimestamp = (*timeAdcMap_iter).first;

            // hit in trigger window
            if (currHitTimestamp > leftTrigWin && currHitTimestamp < rightTrigWin)
            {
               // Fill trigger correlation distribution
               histosObj.hTriggerCorrelation->Fill(currHitTimestamp-currAuxTimestamp);

               // Extract channel and adc information about the current hit
               curChAdcPair = (*timeAdcMap_iter).second;
               currChannel = curChAdcPair.first;
               currAdc = curChAdcPair.second;

               // Increase local counter of hits in the event
               counterOfHitsInEvent++;
               // Account current hit in the distribution of hits per channel without any event information
               histosObj.hNumOfHitsInChannel->Fill(currChannel);

               histosObj.hAdcSpectrum[currChannel]->Fill(currAdc);

               // Erase processed hit
               fTimeAdcMap.erase(timeAdcMap_iter);
               // Increase global counter of erased hits
               counterOfErasedHits++;
            }
            // hit below trigger window
            else if (currHitTimestamp < leftTrigWin)
            {
               // Increase local counter of hits below trigger window
               counterOfHitsBelowEventWin++;
               // Erase processed hit
               fTimeAdcMap.erase(timeAdcMap_iter);
               // Increase global counter of erased hits
               counterOfErasedHits++;
            }
            // hit above trigger window
            else if (currHitTimestamp > rightTrigWin)
            {
               // immediately stop iterating over hit buffer and go to the next trigger message
               break;
            }

         } // End of loop over hits

         // Fill histograms from counters for the current event
         if (counterOfHitsInEvent > 0) {
            histosObj.hNumOfHitsInEvent->Fill(counterOfHitsInEvent);
         }
         if (counterOfHitsBelowEventWin > 0) {
            histosObj.hRemovedHitsBelowWin->Fill(counterOfHitsBelowEventWin);
         }

         // Remove processed trigger message from the buffer
         fTimeAuxMap.erase(timeAuxMap_iter);
         // Increase global counter of erased aux
         counterOfErasedAux++;

      } // End of loop over triggers in the buffer

#ifdef DEBUGMODE
      printf ("\nEnd building event.\n");
#endif
   } // End of condition on the aux buffer length
} // End of CheckAndBuildEvent procedure

