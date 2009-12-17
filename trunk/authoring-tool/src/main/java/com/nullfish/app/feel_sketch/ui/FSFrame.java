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

package com.nullfish.app.feel_sketch.ui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.image.BufferedImage;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.prefs.Preferences;

import javax.imageio.ImageIO;
import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JInternalFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JToolBar;

import jp.co.cia.pmcode.PMCodeEncoderJNI;

import com.nullfish.app.feel_sketch.editor.FSModel;
import com.nullfish.app.feel_sketch.editor.FSPanel;
import com.nullfish.app.feel_sketch.parser.Parser;
import com.nullfish.app.feel_sketch.ui.bg.BGSelectorFrame;
import com.nullfish.app.feel_sketch.ui.layer.LayerSelectorPanel;
import com.nullfish.app.feel_sketch.ui.palette.Palette;

/**
 * The main frame class of Feel Sketch.
 * @author shunji
 *
 */
@SuppressWarnings("serial")
public class FSFrame extends JFrame {
	private FSPanel panel = new FSPanel();
	private Palette palette = new Palette();
	private LayerSelectorPanel layerSelector = new LayerSelectorPanel(panel);
	
	private JMenuBar menuBar = new JMenuBar();
	private JMenu fileMenu = new JMenu(FSResource.resourceBundle.getString("file"));
	private JMenuItem saveMenu = new JMenuItem(new SaveAction());
	private JMenuItem loadMenu = new JMenuItem(new LoadAction());
	private JMenuItem newMenu = new JMenuItem(new NewAction());
	private JMenuItem exportMenu = new JMenuItem(new ExportAction());
	private JMenuItem saveImageMenu = new JMenuItem(new SaveImageAction(false));
	private JMenuItem saveImagePmMenu = new JMenuItem(new SaveImageAction(true));
	private JMenuItem quitMenu = new JMenuItem(new QuitAction());

	private JMenu paletteMenu = new JMenu(FSResource.resourceBundle.getString("palette"));
	private JMenuItem selectMenu = new JMenuItem(new SelectAction());
	private JMenuItem penMenu = new JMenuItem(new PenAction());
	private JMenuItem iconMenu = new JMenuItem(new IconAction());
	private JMenuItem eraserMenu = new JMenuItem(new EraserAction());
	private JMenuItem bgMenu = new JMenuItem(new BGSelectAction());
	private JMenuItem pagePrefMenu = new JMenuItem(new PagePrefAction());
	
	private JToolBar toolBar = new JToolBar() {
		@Override
		public JButton add(Action a) {
			JButton button = super.add(a);
			button.setToolTipText((String) a.getValue(Action.NAME));
			return button;
		};
	};
	
	private BGSelectorFrame bgSelectorFrame = new BGSelectorFrame(panel);
	
	@SuppressWarnings("unused")
	private JInternalFrame iconPaletteFrame = new JInternalFrame();
	
	public static final String SAVE_DIR = "save_dir";
	
	public static final String SAVE_IMAGE_DIR = "save_image_dir";
	
	public FSFrame() {
		initGui();
	}
	
	private void initGui() {
		panel.setLayerSelector(layerSelector);
		
		setTitle("Feel Sketch");
		
		setResizable(false);
		getContentPane().setLayout(new BorderLayout());
		
		this.setJMenuBar(menuBar);
		menuBar.add(fileMenu);
		menuBar.add(paletteMenu);
		
		fileMenu.add(newMenu);
		fileMenu.add(loadMenu);
		fileMenu.add(saveMenu);
		fileMenu.add(saveImageMenu);
		fileMenu.add(saveImagePmMenu);
		fileMenu.add(exportMenu);
		fileMenu.addSeparator();
		fileMenu.add(quitMenu);
		
		paletteMenu.add(selectMenu);
		paletteMenu.add(penMenu);
		paletteMenu.add(iconMenu);
		paletteMenu.add(eraserMenu);
		paletteMenu.add(bgMenu);
		paletteMenu.addSeparator();
		paletteMenu.add(pagePrefMenu);
		
		toolBar.setFloatable(false);
		
		toolBar.add(new NewAction());
		toolBar.add(new SaveAction());
		toolBar.add(new LoadAction());
		toolBar.add(new ExportAction());
		toolBar.add(Box.createHorizontalStrut(10));
		toolBar.add(new SelectAction());
		toolBar.add(new PenAction());
		toolBar.add(new EraserAction());
		toolBar.add(new IconAction());
		toolBar.add(new BGSelectAction());
		toolBar.add(Box.createHorizontalStrut(10));
		toolBar.add(new PagePrefAction());
		
		add(toolBar, BorderLayout.NORTH);
		add(layerSelector, BorderLayout.EAST);

		//panel.setBorder(BorderFactory.createEmptyBorder(3, 3, 3, 3));
		panel.setBorder(BorderFactory.createLineBorder(Color.BLACK, 1));

		panel.addPropertyChangeListener("preferredSize", new PropertyChangeListener() {
			
			@Override
			public void propertyChange(PropertyChangeEvent evt) {
				pack();
			}
		});
		
		JPanel basePanel = new JPanel();
		palette.setOwner(panel);
		basePanel.setLayout(new GridBagLayout());
		basePanel.add(palette, new GridBagConstraints(0, 0, 1, 1, 0.0, 1.0, GridBagConstraints.WEST, GridBagConstraints.BOTH, new Insets(4, 4, 4, 4), 0, 0));
		basePanel.add(panel, new GridBagConstraints(1, 0, 1, 1, 0.0, 1.0, GridBagConstraints.WEST, GridBagConstraints.BOTH, new Insets(4, 4, 4, 4), 0, 0));
		getContentPane().add(basePanel, BorderLayout.CENTER);
		
		palette.startPenMode();
	}
	
	private class SaveAction extends AbstractAction {
		public SaveAction() {
			super(FSResource.resourceBundle.getString("save"), new ImageIcon(FSFrame.this.getClass().getResource("/resource/buttons/save.png")));
		}
		
		@Override
		public void actionPerformed(ActionEvent e) {
			Preferences pref = Preferences.userNodeForPackage(FSFrame.class);
			String saveDirName = pref.get(SAVE_DIR, System.getProperty("user.dir"));
			
			JFileChooser fileChooser = new JFileChooser(saveDirName);
			int result = fileChooser.showSaveDialog(FSFrame.this);
			if(result != JFileChooser.APPROVE_OPTION) {
				return;
			}
			File file = fileChooser.getSelectedFile();
			if(!file.getName().toLowerCase().endsWith(".feelsketch")) {
				file = new File(file.getAbsolutePath()+ ".feelsketch");
			}
			if(file.exists()) {
				result = JOptionPane.showConfirmDialog(FSFrame.this, FSResource.getString("file_exists"), "", JOptionPane.YES_NO_OPTION);
				if(result != JOptionPane.YES_OPTION) {
					return;
				}
			}
			pref.put(SAVE_DIR, file.getAbsolutePath());
			
			ObjectOutputStream oos = null;
			try {
				oos = new ObjectOutputStream(new FileOutputStream(file));
				oos.writeObject(panel.getModel());
			} catch (IOException ex) {
				JOptionPane.showConfirmDialog(FSFrame.this, FSResource.resourceBundle.getString("failed_to_output_file"));
				ex.printStackTrace();
			} finally {
				try {oos.flush();} catch (Exception ex) {}
				try {oos.close();} catch (Exception ex) {}
			}
		}
	}
	
	private class SaveImageAction extends AbstractAction {
		private boolean drawPmCode;
		
		public SaveImageAction(boolean drawPmCode) {
			super(drawPmCode ? 
					FSResource.resourceBundle.getString("save_image_pm") 
					: FSResource.resourceBundle.getString("save_image"), 
				new ImageIcon(FSFrame.this.getClass().getResource("/resource/buttons/save.png")));
			this.drawPmCode = drawPmCode;
		}
		
		@Override
		public void actionPerformed(ActionEvent e) {
			Preferences pref = Preferences.userNodeForPackage(FSFrame.class);
			String saveDirName = pref.get(SAVE_IMAGE_DIR, System.getProperty("user.dir"));
			
			JFileChooser fileChooser = new JFileChooser(saveDirName);
			int result = fileChooser.showSaveDialog(FSFrame.this);
			if(result != JFileChooser.APPROVE_OPTION) {
				return;
			}
			File file = fileChooser.getSelectedFile();
			if(!file.getName().toLowerCase().endsWith(".png")) {
				file = new File(file.getAbsolutePath()+ ".png");
			}
			if(file.exists()) {
				result = JOptionPane.showConfirmDialog(FSFrame.this, FSResource.getString("file_exists"), "", JOptionPane.YES_NO_OPTION);
				if(result != JOptionPane.YES_OPTION) {
					return;
				}
			}
			pref.put(SAVE_IMAGE_DIR, file.getAbsolutePath());
			
			BufferedImage image = panel.toImage(drawPmCode);
			
			try {
				if(drawPmCode) {
					BufferedImage pmCode = createPMCode();
					Graphics g = image.getGraphics();
					Point p = panel.getModel().getPmCodePosition().getPoint(panel.getSize(), new Dimension(pmCode.getWidth(), pmCode.getHeight()));
					g.drawImage(pmCode, p.x, p.y, image.getHeight() / 2, image.getHeight() / 2, FSFrame.this);
					pmCode.flush();
				}

				ImageIO.write(image, "png", file);
			} catch (IOException ex) {
				JOptionPane.showConfirmDialog(FSFrame.this, FSResource.resourceBundle.getString("failed_to_output_file"));
				ex.printStackTrace();
			} finally {
				image.flush();
			}
		}
	}
	
	private class LoadAction extends AbstractAction {
		public LoadAction() {
			super(FSResource.resourceBundle.getString("load"), new ImageIcon(FSFrame.this.getClass().getResource("/resource/buttons/open.png")));
		}

		@Override
		public void actionPerformed(ActionEvent e) {
			Preferences pref = Preferences.userNodeForPackage(FSFrame.class);
			String saveDirName = pref.get(SAVE_DIR, System.getProperty("user.dir"));
			
			JFileChooser fileChooser = new JFileChooser(saveDirName);
			fileChooser.setFileFilter(new javax.swing.filechooser.FileFilter() {
				
				@Override
				public String getDescription() {
					return "Feel Sketch data";
				}
				
				@Override
				public boolean accept(File f) {
					if(f.isDirectory()) return true;
					return f.getName().endsWith(".feelsketch");
				}
			});
			
			int result = fileChooser.showOpenDialog(FSFrame.this);
			if(result != JFileChooser.APPROVE_OPTION) {
				return;
			}
			pref.put(SAVE_DIR, fileChooser.getSelectedFile().getAbsolutePath());
			
			ObjectInputStream ois = null;
			try {
				ois = new ObjectInputStream(new FileInputStream(fileChooser.getSelectedFile()));
				FSModel model = (FSModel) ois.readObject();
				panel.setModel(model);
			} catch (IOException ex) {
				JOptionPane.showConfirmDialog(FSFrame.this, FSResource.resourceBundle.getString("failed_to_read_file"));
				ex.printStackTrace();
			} catch (ClassNotFoundException ex) {
				JOptionPane.showConfirmDialog(FSFrame.this, FSResource.resourceBundle.getString("system_error"));
				ex.printStackTrace();
			} finally {
				try {ois.close();} catch (Exception ex) {}
			}
		}
	}
	
	private class ExportAction extends AbstractAction {
		public ExportAction() {
			super(FSResource.resourceBundle.getString("export"), new ImageIcon(FSFrame.this.getClass().getResource("/resource/buttons/export.png")));
		}

		@Override
		public void actionPerformed(ActionEvent e) {
			Preferences pref = Preferences.userNodeForPackage(FSFrame.class);
			String saveDirName = pref.get(SAVE_DIR, System.getProperty("user.dir"));
			
			JFileChooser fileChooser = new JFileChooser(saveDirName);
			int result = fileChooser.showSaveDialog(FSFrame.this);
			if(result != JFileChooser.APPROVE_OPTION) {
				return;
			}
			File file = fileChooser.getSelectedFile();
			if(!file.getName().toLowerCase().endsWith(".fsext")) {
				file = new File(file.getAbsolutePath()+ ".fsext");
			}
			if(file.exists()) {
				result = JOptionPane.showConfirmDialog(FSFrame.this, FSResource.getString("file_exists"), "", JOptionPane.YES_NO_OPTION);
				if(result != JOptionPane.YES_OPTION) {
					return;
				}
			}
			pref.put(SAVE_DIR, file.getAbsolutePath());
			
			FileOutputStream fos = null;
			try {
				fos = new FileOutputStream(file);
				new Parser().write(panel.getModel(), fos);
			} catch (IOException ex) {
				JOptionPane.showMessageDialog(FSFrame.this, FSResource.resourceBundle.getString("failed_to_output"));
				ex.printStackTrace();
			}
		}
	}
	
	private class NewAction extends AbstractAction {
		public NewAction() {
			super(FSResource.resourceBundle.getString("create_new"), new ImageIcon(FSFrame.this.getClass().getResource("/resource/buttons/file.png")));
		}
		
		@Override
		public void actionPerformed(ActionEvent e) {
			panel.setModel(new FSModel());
		}
		
	}
	
	private class PenAction extends AbstractAction {
		public PenAction() {
			super(FSResource.resourceBundle.getString("pen"), new ImageIcon(FSFrame.this.getClass().getResource("/resource/buttons/pen.png")));
			
		}

		@Override
		public void actionPerformed(ActionEvent e) {
			palette.startPenMode();
		}
	}
	
	private class IconAction extends AbstractAction {
		public IconAction() {
			super(FSResource.resourceBundle.getString("icon"), new ImageIcon(FSFrame.this.getClass().getResource("/resource/buttons/picture.png")));
			
		}

		@Override
		public void actionPerformed(ActionEvent e) {
			palette.startIconMode();
		}
	}
	
	private class EraserAction extends AbstractAction {
		EraserAction() {
			super(FSResource.resourceBundle.getString("eraser"), new ImageIcon(FSFrame.this.getClass().getResource("/resource/buttons/erase.png")));
			
		}

		@Override
		public void actionPerformed(ActionEvent e) {
			palette.startEraserMode();
		}
	}
	
	private class SelectAction extends AbstractAction {
		SelectAction() {
			super(FSResource.getString("select"), new ImageIcon(FSFrame.this.getClass().getResource("/resource/buttons/arrow.png")));
			
		}

		@Override
		public void actionPerformed(ActionEvent e) {
			palette.startSelectMode();
		}
	}

	private class PagePrefAction extends AbstractAction {
		PagePrefAction() {
			super(FSResource.getString("page_preference"), new ImageIcon(FSFrame.this.getClass().getResource("/resource/buttons/prefs.png")));
		}
		
		@Override
		public void actionPerformed(ActionEvent e) {
			palette.startPrefMode();
		}
	}
	
	private class BGSelectAction extends AbstractAction {
		BGSelectAction() {
			super(FSResource.resourceBundle.getString("bg_select"), new ImageIcon(FSFrame.this.getClass().getResource("/resource/buttons/bg.png")));
		}


		@Override
		public void actionPerformed(ActionEvent e) {
			if(!bgSelectorFrame.isVisible()) {
				bgSelectorFrame.setLocationByPlatform(true);
				bgSelectorFrame.pack();
				bgSelectorFrame.selectImage(panel.getModel().getBackgroundImageId());
				bgSelectorFrame.setVisible(true);
			}
			bgSelectorFrame.applyModel();
			bgSelectorFrame.toFront();
		}
	}
	
	private class QuitAction extends AbstractAction {
		QuitAction() {
			super(FSResource.resourceBundle.getString("quit"));
		}


		@Override
		public void actionPerformed(ActionEvent e) {
			System.exit(0);
		}
	}
	
	private BufferedImage createPMCode() throws IOException {
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		new Parser().write(panel.getModel(), bos);

		PMCodeEncoderJNI encoder = new PMCodeEncoderJNI();
		int ret = encoder.encode(bos.toByteArray(), encoder);
		if (ret != 0) {
			JOptionPane.showMessageDialog(FSFrame.this, "Error :" + ret);
			return null;
		}
		
		byte[] image = encoder.getImage();
		int width = encoder.getWidth();
		int height = encoder.getHeight();
		int[] rgbArray = new int[width*height];
		int index = 0;
		for (int rgb_index = 0; rgb_index < rgbArray.length; rgb_index++ ) {
        	rgbArray[rgb_index] |= (image[index] << 16 & 0xFF0000);
        	rgbArray[rgb_index] |= (image[index + 1] << 8 & 0x00FF00);
        	rgbArray[rgb_index] |= (image[index + 2] & 0x0000FF);
            index = index + 3;
	    }
		
		BufferedImage bi = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
		bi.setRGB(0, 0, width, height, rgbArray, 0, width);
		return bi;
	}
}
