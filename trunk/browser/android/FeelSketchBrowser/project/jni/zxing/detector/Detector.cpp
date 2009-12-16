/*
 *  Detector.cpp
 *  zxing
 *
 *  Created by Christian Brunschen on 14/05/2008.
 *  Copyright 2008 ZXing authors All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Detector.h"
#include "BlackPointEstimationMethod.h"
#include "FinderPatternFinder.h"
#include "FinderPattern.h"
#include "AlignmentPattern.h"
#include "AlignmentPatternFinder.h"
#include "Version.h"
#include "GridSampler.h"
#include <cmath>
#include <sstream>

namespace qrcode {
  namespace detector {
    
    using namespace std;
    using namespace common;
    
    Detector::Detector(Ref<MonochromeBitmapSource> image) :
    image_(image) { }
    
    int Detector::detect() {
      if (!BlackPointEstimationMethod_2D == image_->getLastEstimationMethod()) {
        if (image_->estimateBlackPoint(BlackPointEstimationMethod_2D, 0) != 0) {
        	return -1;
        }
      }
      
      FinderPatternFinder finder(image_);
      if (finder.find() != 0) {
    	  return -1;
      }
      Ref<FinderPatternInfo> info(finder.getResult());
      
      Ref<FinderPattern> topLeft(info->getTopLeft());
      Ref<FinderPattern> topRight(info->getTopRight());
      Ref<FinderPattern> bottomLeft(info->getBottomLeft());
      
      float moduleSize = calculateModuleSize(topLeft, topRight, bottomLeft);
      int dimension = 
        computeDimension(topLeft, topRight, bottomLeft, moduleSize);
      if (dimension == 0) {
    	  return -1;
      }
      Version *provisionalVersion = 
        Version::getProvisionalVersionForDimension(dimension);
      if (provisionalVersion == 0) {
    	  return -1;
      }
      int modulesBetweenFPCenters =
        provisionalVersion->getDimensionForVersion() - 7;
      
      Ref<AlignmentPattern> alignmentPattern;
      // Anything above version 1 has an alignment pattern
      if (provisionalVersion->getAlignmentPatternCenters().size() > 0) {
        
        // Guess where a "bottom right" finder pattern would have been
        float bottomRightX =
          topRight->getX() - topLeft->getX() + bottomLeft->getX();
        float bottomRightY =
          topRight->getY() - topLeft->getY() + bottomLeft->getY();
        
        // Estimate that alignment pattern is closer by 3 modules
        // from "bottom right" to known top left location
        float correctionToTopLeft =
          1.0f - 3.0f / (float) modulesBetweenFPCenters;
        int estAlignmentX = (int) (topLeft->getX() + correctionToTopLeft *
                                   (bottomRightX - topLeft->getX()));
        int estAlignmentY = (int) (topLeft->getY() + correctionToTopLeft *
                                   (bottomRightY - topLeft->getY()));
        
        // Kind of arbitrary -- expand search radius before giving up
        for (int i = 4; i <= 16; i <<= 1) {
//          try {
            int error = findAlignmentInRegion(moduleSize,
                                              estAlignmentX,
                                              estAlignmentY,
                                              (float) i);
            if (error == 0) {
            	alignmentPattern = alignmentPattern_;
            	break;
            }
//          } catch (ReaderException *re) {
            // try next round
//            delete re;
//          }
        }
        if (alignmentPattern == 0) {
//          throw new ReaderException("Could not find alignment pattern");
        	return -1;
        }
        
      }
      
      int error = sampleGrid(image_, topLeft, topRight, bottomLeft, alignmentPattern, dimension);
      if (error != 0) {
    	  return error;
      }
      Ref<PointMatrix> points = pointMatrix_;
      ArrayRef<Ref<ResultPoint> > patterns(alignmentPattern == 0 ? 3 : 4);
        patterns[0].reset(bottomLeft);
        patterns[1].reset(topLeft);
        patterns[2].reset(topRight);
      if (alignmentPattern != 0) {
        patterns[3].reset(alignmentPattern);
      }
      Ref<DetectorResult> result(new DetectorResult(points, patterns));
      result_ = result;
      return 0;
    }
    
    int Detector::sampleGrid(Ref<MonochromeBitmapSource> image,
                                        Ref<ResultPoint> topLeft,
                                        Ref<ResultPoint> topRight,
                                        Ref<ResultPoint> bottomLeft,
                                        Ref<ResultPoint> alignmentPattern,
                                        int dimension) {
      float dimMinusThree = (float) dimension - 3.5f;
      float bottomRightX;
      float bottomRightY;
      float sourceBottomRightX;
      float sourceBottomRightY;
      if (alignmentPattern != 0) {
        bottomRightX = alignmentPattern->getX();
        bottomRightY = alignmentPattern->getY();
        sourceBottomRightX = sourceBottomRightY = dimMinusThree - 3.0f;
      } else {
        // Don't have an alignment pattern, just make up the bottom-right point
        bottomRightX = (topRight->getX() - topLeft->getX()) +
          bottomLeft->getX();
        bottomRightY = (topRight->getY() - topLeft->getY()) +
          bottomLeft->getY();
        sourceBottomRightX = sourceBottomRightY = dimMinusThree;
      }
      
      pointMatrix_ = new PointMatrix(dimension);

      GridSampler &sampler = GridSampler::getInstance();
      return sampler.sampleGrid(image,
								pointMatrix_,
                                dimension,
                                3.5f,
                                3.5f,
                                dimMinusThree,
                                3.5f,
                                sourceBottomRightX,
                                sourceBottomRightY,
                                3.5f,
                                dimMinusThree,
                                topLeft->getX(),
                                topLeft->getY(),
                                topRight->getX(),
                                topRight->getY(),
                                bottomRightX,
                                bottomRightY,
                                bottomLeft->getX(),
                                bottomLeft->getY());
    }
    
    int Detector::computeDimension(Ref<ResultPoint> topLeft,
                                   Ref<ResultPoint> topRight,
                                   Ref<ResultPoint> bottomLeft,
                                   float moduleSize) {
      int tltrCentersDimension = round(FinderPatternFinder::distance(topLeft, topRight) / moduleSize);
      int tlblCentersDimension = round(FinderPatternFinder::distance(topLeft, bottomLeft) / moduleSize);
      int dimension = ((tltrCentersDimension + tlblCentersDimension) >> 1) + 7;
      switch (dimension & 0x03) { // mod 4
        case 0:
          dimension++;
          break;
          // 1? do nothing
        case 2:
          dimension--;
          break;
        case 3:
          ostringstream s;
          s << "Bad dimension: " << dimension;
//          throw new ReaderException(s.str().c_str());
          return 0;
      }
      return dimension;
    }
    
    float Detector::calculateModuleSize(Ref<ResultPoint> topLeft, 
                                        Ref<ResultPoint> topRight, 
                                        Ref<ResultPoint> bottomLeft) {
      // Take the average
      return (calculateModuleSizeOneWay(topLeft, topRight) +
              calculateModuleSizeOneWay(topLeft, bottomLeft)) / 2.0f;
    }
    
    float Detector::calculateModuleSizeOneWay(Ref<ResultPoint> pattern, 
                                              Ref<ResultPoint> otherPattern) {
      float moduleSizeEst1 = 
      sizeOfBlackWhiteBlackRunBothWays((int) pattern->getX(),
                                       (int) pattern->getY(),
                                       (int) otherPattern->getX(),
                                       (int) otherPattern->getY());
      float moduleSizeEst2 = 
      sizeOfBlackWhiteBlackRunBothWays((int) otherPattern->getX(),
                                       (int) otherPattern->getY(),
                                       (int) pattern->getX(),
                                       (int) pattern->getY());
      if (isnan(moduleSizeEst1)) {
        return moduleSizeEst2;
      }
      if (isnan(moduleSizeEst2)) {
        return moduleSizeEst1;
      }
      // Average them, and divide by 7 since we've counted the width of 3 black modules,
      // and 1 white and 1 black module on either side. Ergo, divide sum by 14.
      return (moduleSizeEst1 + moduleSizeEst2) / 14.0f;
    }
    
    float Detector::sizeOfBlackWhiteBlackRunBothWays(int fromX, int fromY, 
                                                     int toX, int toY) {
      
      float result = sizeOfBlackWhiteBlackRun(fromX, fromY, toX, toY);
      
      // Now count other way -- don't run off image though of course
      size_t otherToX = fromX - (toX - fromX);
      if (otherToX < 0) {
        // "to" should the be the first value not included, so, the first value off
        // the edge is -1
        otherToX = -1;
      } else if (otherToX >= image_->getWidth()) {
        otherToX = image_->getWidth();
      }
      size_t otherToY = fromY - (toY - fromY);
      if (otherToY < 0) {
        otherToY = -1;
      } else if (otherToY >= image_->getHeight()) {
        otherToY = image_->getHeight();
      }
      result += sizeOfBlackWhiteBlackRun(fromX, fromY, otherToX, otherToY);
      return result - 1.0f; // -1 because we counted the middle pixel twice
    }
    
    float Detector::sizeOfBlackWhiteBlackRun(int fromX, int fromY, 
                                             int toX, int toY) {
      // Mild variant of Bresenham's algorithm;
      // see http://en.wikipedia.org/wiki/Bresenham's_line_algorithm
      bool steep = abs(toY - fromY) > abs(toX - fromX);
      if (steep) {
        int temp = fromX;
        fromX = fromY;
        fromY = temp;
        temp = toX;
        toX = toY;
        toY = temp;
      }
      
      int dx = abs(toX - fromX);
      int dy = abs(toY - fromY);
      int error = -dx >> 1;
      int ystep = fromY < toY ? 1 : -1;
      int xstep = fromX < toX ? 1 : -1;
      int state = 0; // In black pixels, looking for white, first or second time
      for (int x = fromX, y = fromY; x != toX; x += xstep) {
        
        int realX = steep ? y : x;
        int realY = steep ? x : y;
        if (state == 1) { // In white pixels, looking for black
          if (image_->isBlack(realX, realY)) {
            state++;
          }
        } else {
          if (!image_->isBlack(realX, realY)) {
            state++;
          }
        }
        
        if (state == 3) { // Found black, white, black, and stumbled back onto white; done
          int diffX = x - fromX;
          int diffY = y - fromY;
          return (float) sqrt((double) (diffX * diffX + diffY * diffY));
        }
        error += dy;
        if (error > 0) {
          y += ystep;
          error -= dx;
        }
      }
      int diffX = toX - fromX;
      int diffY = toY - fromY;
      return (float) sqrt((double) (diffX * diffX + diffY * diffY));
    }
    
    int
    Detector::findAlignmentInRegion(float overallEstModuleSize,
                                    int estAlignmentX,
                                    int estAlignmentY,
                                    float allowanceFactor) {
      // Look for an alignment pattern (3 modules in size) around where it
      // should be
      int allowance = (int) (allowanceFactor * overallEstModuleSize);
      int alignmentAreaLeftX = max(0, estAlignmentX - allowance);
      int alignmentAreaRightX = min((int) (image_->getWidth() - 1),
                                    estAlignmentX + allowance);
      int alignmentAreaTopY = max(0, estAlignmentY - allowance);
      int alignmentAreaBottomY = min((int) (image_->getHeight() - 1), 
                                     estAlignmentY + allowance);
      
      AlignmentPatternFinder
      alignmentFinder(image_,
                      alignmentAreaLeftX,
                      alignmentAreaTopY,
                      alignmentAreaRightX - alignmentAreaLeftX,
                      alignmentAreaBottomY - alignmentAreaTopY,
                      overallEstModuleSize);
      int error = alignmentFinder.find();
      if (error != 0) {
    	  return error;
      } else {
    	  alignmentPattern_ = alignmentFinder.getResult();
          return 0;
      }
    }
    
  }
}
