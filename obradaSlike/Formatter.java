package obradaSlike;

import java.awt.image.BufferedImage;

public abstract class Formatter {
	
	public abstract void write(BufferedImage image,String destination) throws ErrorOccurred;	
	
	public abstract BufferedImage read(String source) throws ErrorOccurred;
}
