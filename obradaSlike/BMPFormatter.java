package obradaSlike;

import java.awt.image.BufferedImage;
import java.awt.image.RenderedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.imageio.ImageIO;

public class BMPFormatter extends Formatter {

	@Override
	public void write(BufferedImage image, String destination) throws ErrorOccurred {

		try {
			Formatter xml = new XMLFormatter();
			String xmlDest = destination.substring(0,destination.length() - 3) + "xml";
			BufferedImage im = swapRB(image);
			
			xml.write(im,xmlDest);
			
			String commandArgs = "data\\C++\\Debug\\POOP - obradaSlike.exe " + xmlDest + " 1 2";
			
			Runtime runtime = Runtime.getRuntime();
			
			Process process = runtime.exec(commandArgs);
			
			process.waitFor();
			
		} catch(InterruptedException | IOException e) {
			throw new ErrorOccurred("Error writting file!");
		}
	}

	@Override
	public BufferedImage read(String source) throws ErrorOccurred {		
		
		try {		
			String commandArgs = "data\\C++\\Debug\\POOP - obradaSlike.exe " + source;
			
			Runtime runtime = Runtime.getRuntime();
			
			Process process = runtime.exec(commandArgs);
			
			process.waitFor();
			
			source = source.substring(0,source.length() - 3) + "xml";
			
			Formatter xml = new XMLFormatter();
			
			BufferedImage image = xml.read(source);
			
			return swapRB(image);
			
		} catch(InterruptedException | IOException e) {
			throw new ErrorOccurred("Error reading file!");
		}
	}
	
	private BufferedImage swapRB(BufferedImage image) {
		
		BufferedImage retIm = new BufferedImage(image.getWidth(),image.getHeight(),BufferedImage.TYPE_INT_ARGB);
		
		for (int i = 0; i < image.getHeight() ; i++)
			for (int j = 0; j < image.getWidth(); j++) {
				Pixel pix = Pixel.makePixel(image.getRGB(j, i));
				int temp = pix.getBlue();
				pix.setBlue(pix.getRed());
				pix.setRed(temp);
				retIm.setRGB(j, i, Pixel.makeARGB(pix));
			}
		
		return retIm;
	}
}
