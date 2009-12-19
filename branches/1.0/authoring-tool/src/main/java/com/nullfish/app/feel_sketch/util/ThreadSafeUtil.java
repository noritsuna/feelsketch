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

import java.lang.reflect.InvocationTargetException;

import javax.swing.SwingUtilities;

/**
 * A utility class of thread safe execution.
 * It executes Runnable instances on the event dispatch thread anytime.
 * @author shunji
 */
public class ThreadSafeUtil {
	public static void executeRunnable(Runnable runnable) {
		executeRunnable(runnable, true);
	}
	
	public static void executeRunnable(Runnable runnable, boolean waits) {
		if(SwingUtilities.isEventDispatchThread()) {
			runnable.run();
		} else {
			try {
				if(waits) {
					SwingUtilities.invokeAndWait(runnable);
				} else {
					SwingUtilities.invokeLater(runnable);
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				e.printStackTrace();
			}
		}
	}
	
	public static Object executeReturnableRunnable(ReturnableRunnable runnable) {
		if(SwingUtilities.isEventDispatchThread()) {
			runnable.run();
		} else {
			try {
				SwingUtilities.invokeAndWait(runnable);
			} catch (InterruptedException e) {
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				e.printStackTrace();
			}
		}
		
		return runnable.getReturnValue();
	}
}
