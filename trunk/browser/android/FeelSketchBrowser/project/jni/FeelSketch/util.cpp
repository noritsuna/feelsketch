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
#include "util.h"

int get_max_index(double src[], int size_src)
{
    int max_index = 0;
    double tmp_max = src[0];
    for (int i = 1; i < size_src; i++) {
        if (src[i] > tmp_max) {
            tmp_max = src[i];
            max_index = i;
        }
    }
    return max_index;
}

int get_max_index_int(int src[], int size_src)
{
    int max_index = 0;
    int tmp_max = src[0];
    for (int i = 1; i < size_src; i++) {
        if (src[i] > tmp_max) {
            tmp_max = src[i];
            max_index = i;
        }
    }
    return max_index;
}
