#pragma once

#define GE_BIND_CALLBACK_FN(fn) std::bind(&fn, this, std::placeholders::_1)