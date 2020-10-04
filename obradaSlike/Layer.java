package obradaSlike;

import java.awt.image.BufferedImage;

public class Layer {
	
	private BufferedImage image;
	private boolean visible = true;
	private boolean active = true;
	private double transparency = 1;	//range from 0-1(1 means it's not transparent)
	private static int ID = 0;
	private int id = ID++;
	
	public boolean isActive() { return active; }
	
	public double getTransparency() { return transparency; }
	
	public boolean isVisible(){ return visible; }
	
	public int getWidth(){ return image.getWidth(); }
	
	public int getHeight() { return image.getHeight(); }

	public int getId() { return id; }
	
	public void setActive(boolean val) { active = val; }
	
	public void setVisible(boolean val) { visible = val; }
	
	public void setTransparency(double tr) { transparency = (tr > 1 || tr < 0) ? 1 : tr; }
	
	public BufferedImage getImage() {
		return image;
	}
	
	public Layer(String source) throws ErrorOccurred {
		String format = source.substring(source.length() - 3);
		Formatter formatter = null;
		
		if (format.equals("bmp"))
			formatter = new BMPFormatter();
		else if (format.equals("pam"))
			formatter = new PAMFormatter();
		else if (format.equals("xml"))
			formatter = new XMLFormatter();
		else
			throw new ErrorOccurred("File format not recognized");
		
		image = formatter.read(source);
	}
	
	public Layer(int height,int width) {
		image = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
		int val = (0xff << 24) | (0xff << 16) | (0xff << 8) | (0xff);
		
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width ; j++) {
				image.setRGB(j, i, val);
			}
		}
	}
	
	public void increaseSize(int maxWidth,int maxHeight) {
		if (maxWidth == image.getWidth() && maxHeight == image.getHeight())
			return;
		
		BufferedImage newImage = new BufferedImage(maxWidth,maxHeight,BufferedImage.TYPE_INT_ARGB);
		
		for (int i = 0; i < image.getHeight(); i++) {
			for (int j = 0; j < image.getWidth(); j++) {
				newImage.setRGB(j, i, image.getRGB(j, i));
			}
		}
		
		int transpPixel = (0xff << 16) | (0xff << 8) | (0xff);
		
		for (int i = image.getHeight() ; i < newImage.getHeight() ; i++) {
			for (int j = image.getWidth() ; j < newImage.getWidth() ; j++) {
				newImage.setRGB(j, i, transpPixel);
			}
		}
		
		image = newImage;
	}
	
}
