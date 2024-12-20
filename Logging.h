#pragma once
#include <iostream>

using namespace std;

class redirect_output_wrapper
{
    ostream& stream;
    streambuf* const old_buf;
public:
    redirect_output_wrapper(ostream& src)
        :old_buf(src.rdbuf()), stream(src)
    {
    }

    ~redirect_output_wrapper() {
        stream.rdbuf(old_buf);
    }
    void redirect(ostream& dest)
    {
        stream.rdbuf(dest.rdbuf());
    }
};