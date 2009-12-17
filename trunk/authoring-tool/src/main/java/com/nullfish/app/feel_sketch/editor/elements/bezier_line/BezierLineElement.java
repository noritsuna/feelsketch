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

import java.awt.BasicStroke;
import java.awt.Composite;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.Shape;
import java.awt.Stroke;
import java.awt.geom.Area;
import java.awt.geom.Path2D;
import java.awt.geom.Point2D;
import java.util.List;

import com.nullfish.app.feel_sketch.editor.AbstractElement;
import com.nullfish.app.feel_sketch.editor.Layer;
import com.nullfish.app.feel_sketch.editor.Pen;

/**
 * A class of bezier line.
 * @author shunji
 *
 */
public class BezierLineElement extends AbstractElement {
	/**
	 * A shape of this object.
	 */
	private Area area = new Area();
	
	/**
	 * A line which is drawn in bezier line.
	 */
	private Path2D path = new Path2D.Double();

	/**
	 * A shape of the bezier line.
	 */
	private Area bezierArea = new Area();
	
	/**
	 * A line which hasn't become a bezier line yet.
	 */
	private Path2D newLine = new Path2D.Double();
	
	/**
	 * A stroke of the line.
	 */
	private Stroke stroke;
	
	/**
	 * Alpha composite of the line.
	 */
	private Composite composite;

	public BezierLineElement(BezierLineElementModel model, Layer layer) {
		super(model, layer);
		
		stroke = new BasicStroke(model.getPen().getRealWidth(), BasicStroke.CAP_ROUND, BasicStroke.JOIN_BEVEL);
		
		composite = model.getPen().getComposite();
		
		for(Point2D[] line : model.getPoints()) {
			bezierAdded(line);
		}
		
		for(Point2D p : model.getNewPoints()) {
			pointAdded(p);
		}
	}

	@Override
	public void paint(Graphics2D g) {
		Composite orgComposite = g.getComposite();
		
		g.setColor(((BezierLineElementModel)getModel()).getColor());
		g.setStroke(stroke);
		g.setComposite(composite);
		if(g.getClipBounds() == null || bezierArea.intersects(g.getClipBounds())) {
			g.draw(path);
		}
		
		g.draw(newLine);
		g.setComposite(orgComposite);
/*		
		Rectangle clip = g.getClipBounds();
		List<Point2D[]> pointList = ((BezierLineElementModel)getModel()).getPoints();
		for(Point2D[] ps : pointList) {
			g.setColor(Color.BLUE);
			g.fillRect((int)ps[0].getX() - 2, (int)ps[0].getY() - 2, 4, 4);
			g.setColor(Color.RED);
			g.fillRect((int)ps[1].getX() - 2, (int)ps[1].getY() - 2, 4, 4);
			g.fillRect((int)ps[2].getX() - 2, (int)ps[2].getY() - 2, 4, 4);
		}
		
		g.setColor(Color.BLUE);
		if(pointList.size() > 0) {
			g.fillRect(
					(int)pointList.get(pointList.size() - 1)[3].getX() - 2,
					(int)pointList.get(pointList.size() - 1)[3].getY() - 2,
					4, 4);
		}
*/
	}


	@Override
	public void modelAdded(String name, Object added) {
		if(name.equals(BezierLineElementModel.BEZIER)) {
			Point2D[] points = (Point2D[]) added;
			bezierAdded(points);
		}
	}

	private void bezierAdded(Point2D[] points) {
		if(path.getCurrentPoint() == null) {
			path.moveTo(points[0].getX(), points[0].getY());
		}
		path.curveTo(
				points[1].getX(), points[1].getY(),
				points[2].getX(), points[2].getY(),
				points[3].getX(), points[3].getY());

		Path2D newBezier = new Path2D.Double();
		newBezier.moveTo(points[0].getX(), points[0].getY());
		newBezier.curveTo(
				points[1].getX(), points[1].getY(),
				points[2].getX(), points[2].getY(),
				points[3].getX(), points[3].getY());
		
		Shape shape = stroke.createStrokedShape(path);
		bezierArea = new Area(shape);
		Rectangle bounds = shape.getBounds();
		addDirtyRegion(bounds);
		
		clearShape();
		getShape();
	}

	@Override
	public void modelChanged(String name, Object oldValue, Object newValue) {
		if(name.equals(BezierLineElementModel.COLOR)) {
			clearShape();
			repaint();
		} else if(name.equals(BezierLineElementModel.END_POINT)) {
			Point2D newPoint = (Point2D) newValue;
			
			pointAdded(newPoint);
		} else if(name.equals(BezierLineElementModel.PEN)) {
			Pen pen = (Pen)newValue;
			stroke = new BasicStroke(pen.getRealWidth(), BasicStroke.CAP_ROUND, BasicStroke.JOIN_BEVEL);
			addDirtyRegion(stroke.createStrokedShape(path).getBounds());
			clearShape();
			repaint();
		}
	}

	private void pointAdded(Point2D newPoint) {
		Point2D oldPoint = newLine.getCurrentPoint();
		if(oldPoint != null) {
			newLine.lineTo(newPoint.getX(), newPoint.getY());
		} else {
			newLine.moveTo(newPoint.getX(), newPoint.getY());
		}
		
		Rectangle rect = new Rectangle();
		rect.x = (int)newPoint.getX();
		rect.y = (int)newPoint.getY();
		if(oldPoint != null) {
			rect.add(oldPoint);
		}
		
		int width = ((BezierLineElementModel)getModel()).getPen().getRealWidth();
		rect.add(rect.x - width, rect.y - width);
		rect.add(rect.x + rect.width + width, rect.y + rect.width + width);
		addDirtyRegion(rect);

		clearShape();
	}
	
	private void repaint() {
		addDirtyRegion(bezierArea.getBounds());
	}

	@Override
	public void modelRemoved(String name, Object removed) {
		if(name.equals(BezierLineElementModel.NEW_LINE)) {
			addDirtyRegion(stroke.createStrokedShape(newLine).getBounds());
			newLine = new Path2D.Double();
			List<Point2D> points = ((BezierLineElementModel)getModel()).getNewPoints();
			if(points.size() > 0) {
				newLine.moveTo(points.get(0).getX(), points.get(0).getY());
				for(int i=1; i<points.size(); i++) {
					newLine.lineTo(points.get(1).getX(), points.get(1).getY());
				}
			}
			clearShape();
		}
	}

	@Override
	protected Shape getCurrentShape() {
		area.reset();
		area.add(bezierArea);
		if(newLine.getCurrentPoint() != null) {
			area.add(new Area(stroke.createStrokedShape(newLine)));
		}
		
		return area;
	}
}
