#include <vpr/Util/Interval.h>

const vpr::Interval vpr::Interval::NoWait(0,vpr::Interval::USEC);
const vpr::Interval vpr::Interval::NoTimeout(0xffffffffUL,vpr::Interval::USEC);

