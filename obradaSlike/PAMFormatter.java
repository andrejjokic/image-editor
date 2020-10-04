package obradaSlike;

import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class PAMFormatter extends Formatter {

	@Override
	public void write(BufferedImage image, String destination) throws ErrorOccurred {

		try {
			OutputStream output = new FileOutputStream(destination);
			String header = 
					"P7\n" + "WIDTH " + image.getWidth() + "\nHEIGHT " + image.getHeight() +
					"\nDEPTH 4\nMAXVAL 255\nTUPLTYPE RGB_ALPHA\nENDHDR\n";
			
			output.write(header.getBytes());
			for (int i = 0; i < image.getHeight(); i++)
				for (int j = 0; j < image.getWidth(); j++) {
					Pixel pix = Pixel.makePixel(image.getRGB(j, i));
					output.write(pix.getRed());
					output.write(pix.getGreen());
					output.write(pix.getBlue());
					output.write(pix.getAlpha());
				}
			
			output.close();
			
		} catch (IOException e) {
			throw new ErrorOccurred("File can't be written!");
		}
	}

	@Override
	public BufferedImage read(String source) throws ErrorOccurred {
		BufferedImage image = null;
		
		try {
			InputStream input = new FileInputStream(source);
			PAMHeader header = readHeader(input);
			
			image = new BufferedImage(header.width,header.height,BufferedImage.TYPE_INT_ARGB);
			Pixel pix = null;
			int r,g,b,a;
			
			for (int i = 0; i < header.height * header.width * header.depth; i += header.depth) {
				switch (header.depth) {
				case 1:
					r = g = b = input.read();
					pix = new Pixel(r,g,b);
					break;
				case 2:
					r = g = b = input.read();
					a = input.read();
					pix = new Pixel(r,g,b,a);
					break;
				case 3:
					r = input.read();
					g = input.read();
					b = input.read();
					pix = new Pixel(r,g,b);
					break;
				case 4:
					r = input.read();
					g = input.read();
					b = input.read();
					a = input.read();
					pix = new Pixel(r,g,b,a);
					break;
				}
				
				int poz = i / header.depth;
				
				image.setRGB(poz % header.width, poz / header.width, Pixel.makeARGB(pix));
			}
			
			input.close();
			
		} catch (IOException e) {
			throw new ErrorOccurred("Error reading file!");
		}
		
		return image;
	}
	
	private PAMHeader readHeader(InputStream input) throws IOException,ErrorOccurred {
		
		StringBuilder line;
		String[] lines = new String[7];
		int c;
		
		for (int i = 0; i < 7; i++) {
			line = new StringBuilder();
			while ((c = input.read()) != 10) {
				line.append((char) c);
			}
			lines[i] = line.toString();
		}
		
		int w = 0, h = 0 , d = 0;
		String type = null;
		
		Pattern wp = Pattern.compile("^WIDTH (.*)$");	//width
		Matcher wm = wp.matcher(lines[1]);
		if (wm.matches())
			w = Integer.parseInt(wm.group(1));
		else 
			throw new ErrorOccurred("Can't read file!");
		
		Pattern hp = Pattern.compile("^HEIGHT (.*)$");	//height
		Matcher hm = hp.matcher(lines[2]);
		if (hm.matches())
			h = Integer.parseInt(hm.group(1));
		else 
			throw new ErrorOccurred("Can't read file!");
		
		Pattern dp = Pattern.compile("^DEPTH (.*)$");	//depth
		Matcher dm = dp.matcher(lines[3]);
		if (dm.matches())
			d = Integer.parseInt(dm.group(1));
		else 
			throw new ErrorOccurred("Can't read file!");
			
		Pattern tp = Pattern.compile("^TUPLTYPE (.*)$");	//type
		Matcher tm = tp.matcher(lines[5]);
		if (tm.matches())
			type = tm.group(1);
		else 
			throw new ErrorOccurred("Can't read file!");
		
		return new PAMHeader(w,h,d,type);
	}

	static class PAMHeader {
		public String type;
		public int width , height , depth;
		
		public PAMHeader(int width, int height, int depth, String type) {
			super();
			this.type = type;
			this.width = width;
			this.height = height;
			this.depth = depth;
		}	
	}

}
