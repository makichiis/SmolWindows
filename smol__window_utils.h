#ifndef __SMOLINE__WINDOW_UTILS_H__
#define __SMOLINE__WINDOW_UTILS_H__

#define RectResolutionEquals(rect1, rect2) (rect1.bottom - rect1.top == rect2.bottom - rect2.top\
                                && rect1.right - rect1.left == rect2.right - rect2.left)
#define RectResolutionHorizontal(rect) (rect.right - rect.left)
#define RectResolutionVertical(rect) (rect.bottom - rect.top)

#endif