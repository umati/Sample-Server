///
/// \file Test_BindStructValue.cpp
/// \author Christian von Arnim
/// \date 11.02.2020
///

#include <gtest/gtest.h>
#include "../OpcUaTypes/LocalizedText.hpp"
#include "../OpcUaTypes/EUInformation.hpp"
#include <Open62541Cpp/UA_String.hpp>
#include "../UmatiServerLib/ConvertStructValue.hpp"

class Test_Exposure : public UmatiServerLib::ConvertStructValue
{
  public:
  using ConvertStructValue::convertToVariantRefl;
};

TEST(SampleServerLib, BindStructure_Basic)
{

  UmatiServerLib::LocalizedText_t oriLocalText {
    .locale = "en-en",
    .text = "BindStructure_Basic"
  };

  UA_Variant variant;
  Test_Exposure::convertToVariantRefl(&oriLocalText, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT], &variant);
  UA_LocalizedText *pLocalizedText = reinterpret_cast<UA_LocalizedText *>(variant.data);
  ASSERT_TRUE(pLocalizedText);
  auto lTxt = UA_LOCALIZEDTEXT_ALLOC("test", "content");
  UA_LocalizedText_clear(&lTxt);

  open62541Cpp::UA_String variant_local(&pLocalizedText->locale);
  open62541Cpp::UA_String variant_text(&pLocalizedText->text);

  EXPECT_EQ(static_cast<std::string>(variant_local), oriLocalText.locale);
  EXPECT_EQ(static_cast<std::string>(variant_text), oriLocalText.text);

  UA_Variant_clear(&variant);
}

TEST(SampleServerLib, BindStructure_Recursive)
{
  UmatiServerLib::EUInformation_t oriEuInformation {
    .NamespaceUri ="eu://meter",
    .UnitId = -1,
    .DisplayName { .locale="", .text="Meter"},
    .Description = { .locale="en", .text="100cm"},
  };

  UA_Variant variant;
  Test_Exposure::convertToVariantRefl(&oriEuInformation, &UA_TYPES[UA_TYPES_EUINFORMATION], &variant);
  UA_EUInformation * pEuInformation = reinterpret_cast<UA_EUInformation *>(variant.data);

  auto newEuInformation = UmatiServerLib::EUInformation_t::fromUa(*pEuInformation);
  EXPECT_EQ(oriEuInformation, newEuInformation);
}
