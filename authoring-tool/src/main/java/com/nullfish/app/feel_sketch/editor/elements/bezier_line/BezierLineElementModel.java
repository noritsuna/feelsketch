/**
***                      "Feel Sketch" authoring tool.
***    Copyright (C) 2009, Shunji Yamaura
***    Copyright (C) 2009, Noritsuna Imamura (noritsuna@siprop.org)
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

package com.nullfish.app.feel_sketch.editor.elements.bezier_line;

import java.awt.geom.Point2D;
import java.io.ByteArrayOutputStream;
import java.util.ArrayList;
import java.util.List;

import com.nullfish.app.feel_sketch.editor.AbstractElementModel;
import com.nullfish.app.feel_sketch.editor.NumberedColor;
import com.nullfish.app.feel_sketch.editor.Pen;
import com.nullfish.app.feel_sketch.editor.FSModel;
import com.nullfish.app.feel_sketch.util.BezierGenerator;

/**
 * A model class of bezier line.
 * @author shunji
 *
 */
@SuppressWarnings("serial")
public class BezierLineElementModel extends AbstractElementModel {
	/**
	 * A list of 4 control points of bezier lines.
	 */
	private List<Point2D[]> bezierPoints = new ArrayList<Point2D[]>();

	/**
	 * A list of points which havent become a bezier line.
	 */
	private List<Point2D> newPoints = new ArrayList<Point2D>();
	
	private Point2D[] prevBezier;

	private NumberedColor color = NumberedColor.BLACK;
	
	private Pen pen = Pen.narrow;
	
	/**
	 * To decrease memory size, increase the second parameter.
	 */
	private BezierGenerator bezierGenerator = new BezierGenerator(1, 4);

	public static final String COLOR = "bezier_color";
	
	public static final String PEN = "bezier_pen";
	
	public static final String END_POINT = "bezier_end_point";
	
	public static final String BEZIER = "bezier_bezier";
	
	public static final String NEW_LINE = "bezier_new_line";
	
	public NumberedColor getColor() {
		return color;
	}

	public void setColor(NumberedColor color) {
		NumberedColor oldColor = this.color;
		this.color = color;
		notifyChange(COLOR, oldColor, color); 
	}

	public Pen getPen() {
		return pen;
	}

	public void setPen(Pen pen) {
		Pen old = this.pen;
		this.pen = pen;
		notifyChange(PEN, old, pen); 
	}

	public List<Point2D[]> getPoints() {
		return bezierPoints;
	}
	
	public List<Point2D> getNewPoints() {
		return newPoints;
	}
	
	public void addNewPoint(Point2D p) {
		Point2D prevPoint = newPoints.size() > 0 ? newPoints.get(newPoints.size() - 1) : null;
		newPoints.add(p);
		
		notifyChange(END_POINT, prevPoint, p);
		
		if(newPoints.size() <= 2) {
			return;
		}
		
		Point2D[] points = new Point2D[newPoints.size()];
		points = newPoints.toArray(points);
		
		Point2D tangent = null;
		if(bezierPoints.size() > 0) {
			Point2D[] ps = bezierPoints.get(bezierPoints.size() - 1);
			tangent = BezierGenerator.ptUnit(BezierGenerator.ptMinus(ps[3], ps[2]));
		}
		
		Point2D[] newBezier = bezierGenerator.fromPoints(points, tangent, null);
		if(newBezier == null && prevBezier != null) {
			List<Point2D> oldPoints = new ArrayList<Point2D>(newPoints);
			Point2D[] newLine = prevBezier;
			
			if(prevBezier != null) {
				bezierPoints.add(prevBezier);
				prevBezier = null;
			}
			
			newPoints = new ArrayList<Point2D>(newPoints.subList(newPoints.size() - 2, newPoints.size() - 1));
			
			notifyAdded(BEZIER, newLine);
			notifyRemoved(NEW_LINE, oldPoints);
		} else {
			prevBezier = newBezier;
		}
	}
	
	public void lineFinished() {
		if(prevBezier != null) {
			bezierPoints.add(prevBezier);
			List<Point2D> oldPoints = new ArrayList<Point2D>(newPoints);
			prevBezier = null;
			newPoints.clear();
			
			notifyAdded(BEZIER, bezierPoints.get(bezierPoints.size() - 1));
			notifyRemoved(NEW_LINE, oldPoints);
		} else if(newPoints.size() > 0) {
			Point2D[] bezier = {
					newPoints.get(0),
					newPoints.get(0),
					newPoints.get(newPoints.size() - 1),
					newPoints.get(newPoints.size() - 1)
			};
			bezierPoints.add(bezier);
		
			prevBezier = null;
			List<Point2D> oldPoints = new ArrayList<Point2D>(newPoints);
			newPoints.clear();
			
			notifyAdded(BEZIER, bezierPoints.get(bezierPoints.size() - 1));
			notifyRemoved(NEW_LINE, oldPoints);
		}
	}

	@Override
	public int getMemorySize() {
		return 3 * 2 * bezierPoints.size() + (bezierPoints.size() > 0 ? 1 : 0);
	}

	@Override
	public byte[] toBytes(FSModel model) {
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		bos.write((byte)3);
		bos.write((byte)pen.getId());
		bos.write((byte)color.getId());
		for(Point2D[] points : bezierPoints) {
			for(int i=0; i<3; i++) {
				Point2D p = points[i];
				bos.write((byte)(p.getX() * 254 / 640));
				bos.write((byte)(p.getY() * 254 / (640 * model.getPaperSize().getHeight() / 254)));
			}
		}
		Point2D p = bezierPoints.get(bezierPoints.size() - 1)[3];
		bos.write((byte)(p.getX() * 254 / 640));
		bos.write((byte)(p.getY() * 254 / (640 * model.getPaperSize().getHeight() / 254)));
		
		bos.write((byte)255);
		bos.write((byte)255);
		
		return bos.toByteArray();
	}

}
