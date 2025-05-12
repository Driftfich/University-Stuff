#ifndef _TEXT_H
#define _TEXT_H

#include "media.h"

enum class BindingType {
    hardcover,
    paperback,
    spiralbound,
    unknown
};

class Text : public Media {
    int pages;
    BindingType bindingType;
}

#endif