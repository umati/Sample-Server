///
/// \file BindStruct.cpp
/// \author Christian von Arnim
/// \date 11.02.2020
///

#include <gtest/gtest.h>
#include "../OpcUaTypes/LocalizedText.hpp"
#include <Open62541Cpp/UA_String.hpp>
#include "../BindStruct.hpp"

TEST(ExampleServerLib, BindStructure_Basic)
{

  open62541Cpp::LocalizedText_t oriLocalText {
    .locale = "en-en",
    .text = "BindStructure_Basic"
  };

  auto variant = internal_bindStruct::convertToVariantRefl(&oriLocalText, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
  UA_LocalizedText *pLocalizedText = reinterpret_cast<UA_LocalizedText *>(variant.data);
  ASSERT_TRUE(pLocalizedText);
  auto lTxt = UA_LOCALIZEDTEXT_ALLOC("test", "content");
  UA_LocalizedText_deleteMembers(&lTxt);

  open62541Cpp::UA_String variant_local(&pLocalizedText->locale);
  open62541Cpp::UA_String variant_text(&pLocalizedText->text);

  EXPECT_EQ(static_cast<std::string>(variant_local), oriLocalText.locale);
  EXPECT_EQ(static_cast<std::string>(variant_text), oriLocalText.text);

  UA_Variant_deleteMembers(&variant);
}
