#pragma once

namespace art {
   struct BitString;
   struct BitStringChar;
   struct SubtypeCheckInfo;
   enum class ClassStatus : uint8_t;
   enum class InstructionSet;

   namespace mirror {
      struct Object;
   }
}

class Discard
{
   public:
      Discard& operator<< (const char*) { return *this; };
      Discard& operator<< (const std::string) { return *this; };
      Discard& operator<< (unsigned char) { return *this; };
      Discard& operator<< (const art::BitStringChar&) { return *this; };
      Discard& operator<< (const art::BitString&) { return *this; };
      Discard& operator<< (const art::SubtypeCheckInfo&) { return *this; };
      Discard& operator<< (const art::InstructionSet&) { return *this; };
      Discard& operator<< (const art::ClassStatus&) { return *this; };
      Discard& operator<< (const art::mirror::Object *) { return *this; };
};
