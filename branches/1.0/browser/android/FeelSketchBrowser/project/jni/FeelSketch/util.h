/**
***                          "Feel Sketch" browser.
***    Copyright (C) 2009, Content Idea of ASIA Co.,Ltd. (oss.pmcode@ci-a.com)
***
***    This program is free software: you can redistribute it and/or modify
***    it under the terms of the GNU General Public License as published by
***    the Free Software Foundation, either version 3 of the License, or
***    (at your option) any later version.
***
***    This program is distributed in the hope that it will be useful,
***    but WITHOUT ANY WARRANTY; without even the implied warranty of
***    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
***    GNU General Public License for more details.
***
***    You should have received a copy of the GNU General Public License
***    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <android/log.h>

#define length(list) (sizeof(list) / sizeof(list[0]))

int get_max_index(double src[], int size_src);
int get_max_index_int(int src[], int size_src);

#define LOG_TAG "PMCode"
#define LOG_FUNC_MESSAGE(msg) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "function:%s(%s:%d):%s", __FUNCTION__, __FILE__, __LINE__, msg)
#define LOG_FUNC_START __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "function:%s(%s:%d) START!", __FUNCTION__, __FILE__, __LINE__)
#define LOG_FUNC_END   __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "function:%s(%s:%d) END!", __FUNCTION__, __FILE__, __LINE__)
#define LOGV(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// i: IplImage, x: X, y: Y, n: channel(0:blue, 1:green, 2:red)
#define IMAGE( i, x, y, n )   *(( unsigned char * )(( i )->imageData      \
                                    + ( x ) * sizeof( unsigned char ) * 3 \
                                    + ( y ) * ( i )->widthStep ) + ( n ))
