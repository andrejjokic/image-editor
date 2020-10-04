package obradaSlike;

import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class XMLFormatter extends Formatter {

	@Override
	public void write(BufferedImage image, String destination) throws ErrorOccurred {
		
		try {
			FileWriter fileWriter = new FileWriter(destination);
			StringBuilder text = new StringBuilder();
			
			text.append("<Height>" + image.getHeight() + "</Height>\n");
			
			text.append("<Width>" + image.getWidth() + "</Width>\n");
			
			text.append("<Pixels>\n");
			
			for (int i = 0; i < image.getHeight(); i++) 
				for (int j = 0; j < image.getWidth(); j++) {
					Pixel pix = Pixel.makePixel(image.getRGB(j, i));
					
					text.append(pix.getRed() + " ");
					text.append(pix.getGreen() + " ");
					text.append(pix.getBlue() + " ");
					text.append(pix.getAlpha() + " ");
				}
			
			text.append("\n</Pixel>");
			
			fileWriter.write(text.toString());
			
			fileWriter.close();
			
		} catch (IOException e) {
			throw new ErrorOccurred("Error writting file!");
		}
	}

	@Override
	public BufferedImage read(String source) throws ErrorOccurred {
		BufferedImage image = null;
		
		try {
			FileReader fileReader = new FileReader(source);
			BufferedReader buffReader = new BufferedReader(fileReader);
			
			String hText = buffReader.readLine();
			Pattern patH = Pattern.compile("^<Height>([^<]*)</Height>$");
			Matcher matcherH = patH.matcher(hText);
			
			String wText = buffReader.readLine();
			Pattern patW = Pattern.compile("^<Width>([^<]*)</Width>$");
			Matcher matcherW = patW.matcher(wText);
			
			matcherW.matches();
			matcherH.matches();
			
			int h = Integer.parseInt(matcherH.group(1));
			int w = Integer.parseInt(matcherW.group(1));
			
			buffReader.readLine();	//<Pixels>
			
			String pixText = buffReader.readLine();
			
			String[] components = pixText.split(" ");
			
			image = new BufferedImage(w,h,BufferedImage.TYPE_INT_ARGB);
			
			for (int i = 0; i < components.length;) {
				int index = i / 4;
				
				int r = Integer.parseInt(components[i++]);
				int g = Integer.parseInt(components[i++]);
				int b = Integer.parseInt(components[i++]);
				int a = Integer.parseInt(components[i++]);
				
				image.setRGB(index % w, index / w, Pixel.makeARGB(new Pixel(r,g,b,a)));
			}

			buffReader.close();
			
		} catch (IOException e) {
			throw new ErrorOccurred("Error reading file!");
		}
		
		return image;
	}
	
	public String writeLayer(Layer layer) {
		StringBuilder text = new StringBuilder();
		
		text.append("<Layer>\n");
		
		text.append("<active>" + (layer.isActive() ? 1 : 0) + "</active>\n");
		
		text.append("<visible>" + (layer.isVisible() ? 1 : 0) + "</visible>\n");
		
		text.append("<transparency>" + ((int)(layer.getTransparency() * 100)) + "</transparency>\n");
		
		String fileName = "layer" + layer.getId() + ".xml";
		
		try {
			write(layer.getImage(),"data\\" + fileName);
		} catch (ErrorOccurred e) {}
		
		text.append("<path>" + "data\\" + fileName + "</path>\n");
		
		text.append("</Layer>\n");
		
		return text.toString();
	}
	
	public String writeSelection(Selection selection) {
		StringBuilder text = new StringBuilder();
		
		text.append("<Selection>\n");
		
		text.append("<name>" + selection.getName() + "</name>\n");
		
		text.append("<active>" + (selection.isActive() ? 1 : 0) + "</active>\n");
		
		String fileName = selection.getName() + ".xml";
		
		try {
			FileWriter fileWriter = new FileWriter("data\\" + fileName);
			StringBuilder pointsText = new StringBuilder();
			
			selection.getAllPoints().stream().forEach(p -> {
				pointsText.append(p.getX() + "," + p.getY() + " ");		
			});
			
			if (pointsText.length() > 1)
				pointsText.deleteCharAt(pointsText.length() - 1);
			
			fileWriter.write(pointsText.toString());	
			fileWriter.close();
			
		} catch (IOException e) {}
		
		text.append("<Points>" + "data\\" + fileName + "</Points>\n");	//we add data\ , because we save it in data folder
		
		text.append("</Selection>\n");
		
		return text.toString();
	}
	
	public String writeOperation(Operation operation) {
		
		return operation.writeXML();
	}

}
