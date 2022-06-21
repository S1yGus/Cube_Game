#pragma once

class CGUtils
{
public:
    static FText TextFromInt(int32 Number) { return FText::FromString(FString::FromInt(Number)); }
};