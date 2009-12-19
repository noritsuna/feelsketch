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

package com.nullfish.app.feel_sketch.util;

import java.awt.geom.Point2D;
import java.io.Serializable;

/**
 * A class which generates a bezier line from a set of pixels.
 * @author shunji
 *
 */
@SuppressWarnings("serial")
public class BezierGenerator implements Serializable {
	private Point2D[] bezier;
	private Point2D[] data;
	private double[] params;
	private int length;
	private Point2D startTangent;
	private Point2D endTangent;
	private int splitPoint;
	private int maxIterations;
	private double tolerance;

	public BezierGenerator(int maxIterations, double tolerance) {
		this.maxIterations = maxIterations;
		this.tolerance = tolerance;
	}

	public void setMaxIterations(int mi) {
		maxIterations = mi;
	}

	public void setTolerance(double tr) {
		tolerance = tr;
	}

	public Point2D[] fromPoints(Point2D[] points, Point2D start, Point2D end) {

		setup(points, start, end);
		Point2D[] ret = _fromPoints();
		free();

		return ret;
	}

	void setup(Point2D[] points, Point2D start, Point2D end) {
		bezier = new Point2D[4];
		data = points;
		params = new double[data.length];
		length = data.length;
		startTangent = start != null ? start : new Point2D.Float(0.0f, 0.0f);
		endTangent = end != null ? end : new Point2D.Float(0.0f, 0.0f);
		splitPoint = 0;
	}

	void free() {
		bezier = data = null;
		params = null;
		startTangent = endTangent = null;
	}

	Point2D[] _fromPoints() {
		if (length == 2) {
			return fromTwoPoints();
		}
		return generateBezier();
	}

	Point2D[] fromTwoPoints() {

		bezier[0] = new Point2D.Double(data[0].getX(), data[0].getY());
		bezier[3] = new Point2D.Double(data[1].getX(), data[1].getY());
		final double distance = ptDistance(bezier[0], bezier[3]) / 3;

		bezier[1] = ptIsZero(startTangent) ? ptOperate2(bezier[0], bezier[3],
				new Operate2Interface() {
					public double func(double a, double b) {
						return (a * 2.0f + b) / 3.0f;
					}
				}) : ptOperate2(bezier[0], startTangent,
				new Operate2Interface() {
					public double func(double a, double b) {
						return a + b * distance;
					}
				});
		bezier[2] = ptIsZero(endTangent) ? ptOperate2(bezier[3], bezier[0],
				new Operate2Interface() {
					public double func(double a, double b) {
						return (a * 2.0f + b) / 3.0f;
					}
				}) : ptOperate2(bezier[3], endTangent, new Operate2Interface() {
			public double func(double a, double b) {
				return a + b * distance;
			}
		});
		return bezier;
	}

	Point2D[] generateBezier() {

		setParamsByLength();
		setAssumptBezier();
		double error = maxError();

		if (Math.abs(error) <= 1)
			return bezier;

		if (error >= 0.0f && error <= 3.0f) {
			for (int i = 0; i < maxIterations; i++) {
				setAssumptBezier();
				error = maxError();
				if (Math.abs(error) <= 1.0f)
					return bezier;
			}
		}

		if (error < 0.0f) {
			if (splitPoint == 0 && !ptIsZero(startTangent))
				return fromPoints(data, new Point2D.Float(0.0f, 0.0f),
						endTangent);
			if (splitPoint == length - 1 && !ptIsZero(endTangent))
				return fromPoints(data, startTangent, new Point2D.Float(0.0f,
						0.0f));
		}

		return null;
	}

	void setParamsByLength() {
		int i;
		params[0] = 0;
		for (i = 1; i < length; i++)
			params[i] = params[i - 1] + ptDistance(data[i], data[i - 1]);
		double total = params[length - 1];
		for (i = 0; i < length; i++)
			params[i] /= total;
	}

	void setAssumptBezier() {
		Point2D start = ptIsZero(startTangent) ? estimateStartTangent()
				: startTangent;
		Point2D end = ptIsZero(endTangent) ? estimateEndTangent() : endTangent;

		setAssumptBezierWithTangent(start, end);
		if (ptIsZero(startTangent)) {
			setControllPoint(1);
			if (!ptEqual(bezier[0], bezier[1]))
				start = ptUnit(ptMinus(bezier[1], bezier[0]));
			setAssumptBezierWithTangent(start, end);
		}
		reparameterize();
	}

	// @@@ bezier-util.cpp -> estimate_lengths
	void setAssumptBezierWithTangent(Point2D start, Point2D end) {

		double[][] C = { { 0, 0 }, { 0, 0 } };
		double[] X = { 0, 0 };

		bezier[0] = new Point2D.Double(data[0].getX(), data[0].getY());
		bezier[3] = new Point2D.Double(data[length - 1].getX(),
				data[length - 1].getY());

		for (int i = 0; i < length; i++) {

			double[] b = B(params[i]);
			final double b0 = b[0];
			final double b1 = b[1];
			final double b2 = b[2];
			final double b3 = b[3];
			Point2D a1 = ptTime(start, b1);
			Point2D a2 = ptTime(end, b2);

			C[0][0] += ptDot(a1, a1);
			C[0][1] += ptDot(a1, a2);
			C[1][0] = C[0][1];
			C[1][1] += ptDot(a2, a2);
			Point2D offset = ptOperate3(data[i], bezier[0], bezier[3],
					new Operate3Interface() {
						public double func(double p, double bezier0,
								double bezier3) {
							return p - (b0 + b1) * bezier0 - (b2 + b3)
									* bezier3;
						}
					});

			X[0] += ptDot(a1, offset);
			X[1] += ptDot(a2, offset);
		}

		double alphaL, alphaR;
		double detC = C[0][0] * C[1][1] - C[1][0] * C[0][1];

		if (detC != 0) {
			double detC0X = C[0][0] * X[1] - C[0][1] * X[0];
			double detXC1 = X[0] * C[1][1] - X[1] * C[0][1];

			alphaL = detXC1 / detC;
			alphaR = detC0X / detC;
		} else {
			double c0 = C[0][0] + C[0][1];
			if (c0 != 0) {
				alphaL = X[0] / c0;
			} else {
				double c1 = C[1][0] + C[1][1];
				alphaL = c1 != 0 ? X[1] / c1 : 0;
			}
			alphaR = alphaL;
		}

		if (alphaL < 1e-6 || alphaR < 1e-6)
			alphaL = alphaR = ptDistance(data[0], data[length - 1]) / 3.0f;

		bezier[1] = ptPlus(ptTime(start, alphaL), bezier[0]);
		bezier[2] = ptPlus(ptTime(end, alphaR), bezier[3]);

	}

	void setControllPoint(final int ei) {
		final int oi = 3 - ei;
		Point2D result = new Point2D.Double(0.0d, 0.0d);
		double den = 0;
		for (int i = 0; i < length; i++) {
			final double[] b = B(params[i]);
			result = ptOperate5(result, bezier[0], bezier[3], bezier[oi],
					data[i], new Operate5Interface() {
						public double func(double r, double b0, double b3,
								double bo, double p) {
							return r + b[ei]
									* (b[0] * b0 + b[3] * b3 + b[oi] * bo - p);
						}
					});
			den -= b[ei] * b[ei];
		}
		if (den != 0) {
			result = ptTime(result, 1.0f / den);
		} else {
			result = ptOperate2(bezier[0], bezier[3], new Operate2Interface() {
				public double func(double p0, double p3) {
					return (oi * p0 + ei * p3) / 3.0f;
				}
			});
		}
		bezier[ei] = result;
	}

	Point2D estimateStartTangent() {
		Point2D tangent = null;
		for (int i = 1; i < length; i++) {
			tangent = ptMinus(data[i], data[0]);
			if (ptSquare(tangent) > tolerance * tolerance)
				return ptUnit(tangent);
		}
		return ptIsZero(tangent) ? ptUnit(ptMinus(data[1], data[0]))
				: ptUnit(tangent);
	}

	Point2D estimateEndTangent() {
		Point2D tangent = null;
		for (int i = length - 2; i >= 0; i--) {
			tangent = ptMinus(data[i], data[length - 1]);
			if (ptSquare(tangent) > tolerance * tolerance)
				return ptUnit(tangent);
		}
		return ptIsZero(tangent) ? ptUnit(ptMinus(data[length - 2],
				data[length - 1])) : ptUnit(tangent);
	}

	void reparameterize() {
		for (int i = 0; i < length - 1; i++) {
			params[i] = newtonRaphsonRootFind(bezier, data[i], params[i]);
		}
	}

	double newtonRaphsonRootFind(Point2D[] bezier, Point2D point, double param) {

		Point2D[] dbezier = pointDifference(bezier);
		Point2D[] ddbezier = pointDifference(dbezier);

		Point2D p = bezierPt(bezier, param);
		Point2D dp = bezierPt(dbezier, param);
		Point2D ddp = bezierPt(ddbezier, param);

		Point2D diff = ptMinus(p, point);
		double numerator = ptDot(diff, dp);
		double denominator = ptSquare(dp) + ptDot(diff, ddp);

		double improvedParam;
		if (denominator > 0) {
			improvedParam = param - (numerator / denominator);
		} else {
			if (numerator > 0) {
				improvedParam = param * 0.98f - 0.01f;
			} else if (numerator < 0) {
				improvedParam = param * 0.98f + 0.031f;
			} else {
				improvedParam = param;
			}
		}
		if (improvedParam < 0) {
			improvedParam = 0;
		} else if (improvedParam > 1) {
			improvedParam = 1;
		}

		double diffSquare = ptSquare(diff);
		for (double proportion = 0.125f;; proportion += 0.125f) {
			if (ptSquare(ptMinus(bezierPt(bezier, improvedParam), point)) > diffSquare) {
				if (proportion > 1) {
					improvedParam = param;
					break;
				}
				improvedParam = (1 - proportion) * improvedParam + proportion
						* param;
			} else {
				break;
			}
		}

		return improvedParam;
	}

	double maxError() {
		double toleranceMore = tolerance;
		double maxDistanceSquare = 0;
		double maxHook = 0;
		int snapEnd = 0;
		Point2D prevPoint = bezier[0];

		for (int i = 1; i < length; i++) {
			Point2D currentPoint = bezierPt(bezier, params[i]);
			double distanceSquare = ptSquare(ptMinus(currentPoint, data[i]));
			if (distanceSquare > maxDistanceSquare) {
				maxDistanceSquare = distanceSquare;
				splitPoint = i;
			}
			double hook = computeHook(prevPoint, currentPoint,
					(params[i] + params[i - 1]) / 2);
			if (hook > maxHook) {
				maxHook = hook;
				snapEnd = i;
			}
			prevPoint = currentPoint;
		}

		double maxDistanceRatio = Math.sqrt(maxDistanceSquare) / toleranceMore;

		if (maxHook <= maxDistanceRatio) {
			return (double) maxDistanceRatio;
		} else {
			splitPoint = snapEnd - 1;
			return -maxHook;
		}
	}

	double computeHook(Point2D point1, Point2D point2, double param) {
		double toleranceMore = tolerance;
		double distance = ptAbs(ptOperate3(point1, point2, bezierPt(bezier,
				param), new Operate3Interface() {
			public double func(double p1, double p2, double t) {
				return (p1 + p2) / 2 - t;
			}
		}));
		return (distance < toleranceMore) ? 0 : distance
				/ (ptDistance(point1, point2) + toleranceMore);
	}

	Point2D[] pointDifference(Point2D[] points) {

		int mlen = points.length - 1;
		Point2D[] diff = new Point2D[mlen];
		for (int i = 0; i < mlen; i++)
			diff[i] = ptTime(ptMinus(points[i + 1], points[i]), mlen);
		return diff;
	}

	double[] B(double u) {
		double[] ret = { (1 - u) * (1 - u) * (1 - u),
				3 * u * (1 - u) * (1 - u), 3 * u * u * (1 - u), u * u * u };

		return ret;
	}

	Point2D bezierPt(Point2D[] b, double t) {
		double[][] pascal = { { 1 }, { 1, 1 }, { 1, 2, 1 }, { 1, 3, 3, 1 } };
		double s = 1 - t;
		int degree = b.length - 1;

		double[] spow = new double[degree + 1];
		double[] tpow = new double[degree + 1];
		spow[0] = tpow[0] = 1;

		int d;

		for (d = 0; d < degree; d++) {
			spow[d + 1] = spow[d] * s;
			tpow[d + 1] = tpow[d] * t;
		}

		Point2D result = ptTime(b[0], spow[degree]);

		for (d = 1; d <= degree; d++)
			result = ptPlus(result, ptTime(b[d], pascal[degree][d]
					* spow[degree - d] * tpow[d]));
		return result;
	}

	public static boolean ptEqual(Point2D p1, Point2D p2) {
		return p1.getX() == p2.getX() && p1.getY() == p2.getY();
	}

	public static boolean ptIsZero(Point2D p) {
		return p.getX() == 0.0f && p.getY() == 0.0f;
	}

	public static double ptDistance(Point2D p1, Point2D p2) {
		double x = p1.getX() - p2.getX();
		double y = p1.getY() - p2.getY();
		return Math.sqrt(x * x + y * y);
	}

	public static double ptAbs(Point2D p) {
		return Math.sqrt(p.getX() * p.getX() + p.getY() * p.getY());
	}

	public static double ptDot(Point2D p1, Point2D p2) {
		return p1.getX() * p2.getX() + p1.getY() * p2.getY();
	}

	public static Point2D ptUnit(Point2D p) {
		double abs = ptAbs(p);
		return new Point2D.Double(p.getX() / abs, p.getY() / abs);
	}

	public static double ptSquare(Point2D p) {
		return p.getX() * p.getX() + p.getY() * p.getY();
	}

	public static Point2D ptPlus(Point2D p1, Point2D p2) {
		return new Point2D.Double(p1.getX() + p2.getX(), p1.getY() + p2.getY());
	}

	public static Point2D ptMinus(Point2D p1, Point2D p2) {
		return new Point2D.Double(p1.getX() - p2.getX(), p1.getY() - p2.getY());
	}

	public static Point2D ptTime(Point2D p, double value) {
		return new Point2D.Double(p.getX() * value, p.getY() * value);
	}

	interface Operate2Interface {
		double func(double v1, double v2);
	}

	Point2D ptOperate2(Point2D v1, Point2D v2, Operate2Interface oi) {
		return new Point2D.Double(oi.func(v1.getX(), v2.getX()), oi.func(v1
				.getY(), v2.getY()));
	}

	// 
	interface Operate3Interface {
		double func(double v1, double v2, double v3);
	}

	Point2D ptOperate3(Point2D v1, Point2D v2, Point2D v3, Operate3Interface oi) {
		return new Point2D.Double(oi.func(v1.getX(), v2.getX(), v3.getX()), oi
				.func(v1.getY(), v2.getY(), v3.getY()));
	}

	// 
	interface Operate5Interface {
		double func(double v1, double v2, double v3, double v4, double v5);
	}

	Point2D ptOperate5(Point2D v1, Point2D v2, Point2D v3, Point2D v4,
			Point2D v5, Operate5Interface oi) {
		return new Point2D.Double(oi.func(v1.getX(), v2.getX(), v3.getX(), v4
				.getX(), v5.getX()), oi.func(v1.getY(), v2.getY(), v3.getY(),
				v4.getY(), v5.getY()));
	}
}
