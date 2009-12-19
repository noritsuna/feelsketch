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

package jp.co.cia.feelsketchbrowser;

import android.content.res.Resources;

public class ColorSettings {
	private int[] _3Layers = new int[3];
	public int[] get3Layers() {
		return _3Layers;
	}
	
	private int[] _4Layers = new int[4];
	public int[] get4Layers() {
		return _4Layers;
	}
	
	public ColorSettings(Resources r) {
		// reject alpha 8bit
		int mask = 0x00FFFFFF;
		_3Layers[0] = r.getColor(R.color.layer1_of_3layers) & mask;
		_3Layers[1] = r.getColor(R.color.layer2_of_3layers) & mask;
		_3Layers[2] = r.getColor(R.color.layer3_of_3layers) & mask;

		_4Layers[0] = r.getColor(R.color.layer1_of_4layers) & mask;
		_4Layers[1] = r.getColor(R.color.layer2_of_4layers) & mask;
		_4Layers[2] = r.getColor(R.color.layer3_of_4layers) & mask;
		_4Layers[3] = r.getColor(R.color.layer4_of_4layers) & mask;
	}
}
