package obradaSlike;

import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.CharBuffer;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Stream;

import obradaSlike.BasicOperation.OperationsIds;

public class Image {

	private List<Layer> layers = new LinkedList<Layer>();
	private Map<String,Selection> selections = new HashMap<String,Selection>();
	private Map<String,Operation> operations = new HashMap<String,Operation>();
	private boolean imageSaved = true;
	private boolean projectSaved = true;
	private boolean imageChanged;
	private int maxWidth;
	private int maxHeight;
	private BufferedImage image;
	private int activeSelections;
	
	Matcher matcher;
	
	public Image() {
		try {
			operations.put("Inverse",new BasicOperation("Inverse"));
			operations.put("Gray", new BasicOperation("Gray"));
			operations.put("BlackWhite", new BasicOperation("BlackWhite"));
			operations.put("Median", new BasicOperation("Median"));
		} catch (ErrorOccurred e) {}
	}
	
	public int getWidth() {
		return maxWidth;
	}
	
	public int getHeight() {
		return maxHeight;
	}
	
	private void clearProject() {
		maxWidth = maxHeight = activeSelections = 0;
		image = null;
		layers.clear();
		selections.clear();
		operations.clear();
	}
	
	public BufferedImage getImage() {
		return image;
	}
	
	public Image addLayer(String source) throws ErrorOccurred {
		Layer newLayer = new Layer(source);
		layers.add(newLayer);

		adjustLayers(newLayer);

		imageSaved = projectSaved = false;
		imageChanged = true;

		return this;
	}

	public Image addLayer(int height, int width) throws ErrorOccurred {

		if (height <= 0 || width <= 0)
			throw new ErrorOccurred("Image height or width value is innapropriate!");

		Layer newLayer = new Layer(height, width);
		layers.add(newLayer);
		adjustLayers(newLayer);

		imageSaved = projectSaved = false;
		imageChanged = true;

		return this;
	}
	
	public Image addLayer() throws ErrorOccurred {
		if (layers.size() == 0)
			throw new ErrorOccurred("First layer must have size!");

		Layer newLayer = new Layer(maxHeight, maxWidth);
		layers.add(newLayer);

		imageSaved = false;
		projectSaved = false;

		return this;
	}

	private void adjustLayers(Layer newLayer) {

		if ((maxHeight != newLayer.getHeight()) || (maxWidth != newLayer.getWidth())) {

			maxHeight = (newLayer.getHeight() > maxHeight) ? newLayer.getHeight() : maxHeight;
			maxWidth = (newLayer.getWidth() > maxWidth) ? newLayer.getWidth() : maxWidth;

			layers.stream().forEach(l -> l.increaseSize(maxWidth, maxHeight));
		}
	}
	
	public List<Layer> getLayers() {
		return layers;
	}
	
	public Layer getLayer(int index) throws ErrorOccurred {
		if (index < 0 || index >= layers.size())
			throw new ErrorOccurred("Layer with that number doesn't exist!");
		
		return layers.get(index);
	}
	
	public void setLayerActivity(int index,boolean active) throws ErrorOccurred {
		getLayer(index).setActive(active);
		projectSaved = false;
	}
	
	public void setLayerVisibility(int index,boolean visible) throws ErrorOccurred {
		getLayer(index).setVisible(visible);
		imageSaved = projectSaved = false;
		imageChanged = true;
	}

	public void setLayerTransparency(int index,double transp) throws ErrorOccurred  {
		getLayer(index).setTransparency(transp);
		imageSaved = projectSaved = false;
		imageChanged = true;
	}

	public void removeLayer(int num) throws ErrorOccurred {
		if (num < 0 || num >= layers.size())
			throw new ErrorOccurred("Layer with that number doesn't exist!");
		
		layers.remove(num);
		imageSaved = projectSaved = false;
		imageChanged = true;
	}
	
	public void updateImage() {
		if (!imageChanged)
			return;
		
		if (layers.size() == 0 || layers.stream().allMatch(l -> !l.isVisible())) {
			image = new BufferedImage(maxWidth,maxHeight,BufferedImage.TYPE_INT_ARGB);
			imageChanged = false;
			return;
		}
		
		if (image == null || image.getWidth() != maxWidth || image.getHeight() != maxHeight)
			image = new BufferedImage(maxWidth,maxHeight,BufferedImage.TYPE_INT_ARGB);
		
		int round = 0;
		double charged = 0;
		
		for (Layer lay : layers) {
			
			if (!lay.isVisible())
				continue;
			
			if (charged >= 1)
				break;
			
			if (round == 0) {	//it's first layer so we just copy pixels to our image
				double proc = lay.getTransparency() * (1 - charged);
						
				for (int i = 0; i < maxHeight ; i++) {
					for (int j = 0; j < maxWidth ; j++) {
						
						Pixel pix = Pixel.makePixel(lay.getImage().getRGB(j, i));
						pix.setAlpha((int)(pix.getAlpha() * proc));
						image.setRGB(j,i,Pixel.makeARGB(pix));
					}
				}
				
				charged += proc;
				round++;
				continue;
			}
			
			double proc = lay.getTransparency() * (1 - charged);
			
			for (int i = 0; i < maxHeight ; i++) {
				for (int j = 0; j < maxWidth ; j++) {
					
					Pixel imagePix = Pixel.makePixel(image.getRGB(j, i));
					Pixel layerPix = Pixel.makePixel(lay.getImage().getRGB(j, i));
					
					double imageAlpha = imagePix.getAlpha() / 255.;
					double layerAlpha = layerPix.getAlpha() / 255. * lay.getTransparency();
					
					double At = imageAlpha + (1 - imageAlpha) * layerAlpha;
					int Rt = (int) (imagePix.getRed() * imageAlpha / At + layerPix.getRed() * (1 - imageAlpha) * layerAlpha / At);
					int Gt = (int) (imagePix.getGreen() * imageAlpha / At + layerPix.getGreen() * (1 - imageAlpha) * layerAlpha / At);
					int Bt = (int) (imagePix.getBlue() * imageAlpha / At + layerPix.getBlue() * (1 - imageAlpha) * layerAlpha / At);

					image.setRGB(j, i, Pixel.makeARGB(new Pixel(Rt, Gt, Bt, (int) (At * 255))));
				}
			}
			
			charged += proc;
			round++;
		}

		imageChanged = false;
	}

	public void saveImage(String destination) throws ErrorOccurred {
		String format = destination.substring(destination.length() - 3);
		Formatter formatter;
		
		if (format.equals("bmp"))
			formatter = new BMPFormatter();
		else if (format.equals("xml"))
			formatter = new XMLFormatter();
		else if (format.equals("pam"))
			formatter = new PAMFormatter();
		else
			throw new ErrorOccurred("Format " + format + " is not recognized!");
		
		updateImage();
		formatter.write(image, destination);
		
		imageSaved = true;
	}
	
	public Map<String,Selection> getSelections() {
		return selections;
	}

	public Selection getSelection(String name) throws ErrorOccurred {
		if (!selections.containsKey(name))
			throw new ErrorOccurred("Selection with that name doesn't exist!");
		
		Selection s = selections.get(name);
		return s;
	}

	public void addSelection(Selection selection) {
		selections.put(selection.getName(), selection);
		
		activeSelections++;
		projectSaved = false;
	}
	
	public void addSelection(String name,List<Rectangle> rects) {
		selections.put(name, new Selection(name,rects));
		
		activeSelections++;
		projectSaved = false;
	}

	public void addSelection(String name,Set<Point> points) {
		selections.put(name, new Selection(name,points));
		
		activeSelections++;
		projectSaved = false;
	}
	
	public void setSelectionActivity(String name, boolean active) throws ErrorOccurred {
		
		Selection s = getSelection(name);
		
		if (s.isActive() && !active)
			activeSelections--;
		
		if (!s.isActive() && active)
			activeSelections++;
		
		s.setActive(active);
		
		projectSaved = false;
	}

	public void removeSelection(String name) throws ErrorOccurred {
		Selection s = getSelection(name);
		
		if (s.isActive())
			activeSelections--;
		
		selections.remove(name);
		
		projectSaved = false;
	}
	
	public void colourSelection(String name,int r, int g, int b) throws ErrorOccurred {
		Selection selection = getSelection(name);
		Pixel pix = new Pixel(r,g,b);
		
		selection.getAllPoints().stream().forEach(p -> {
			int i = p.getY();
			int j = p.getX();
			
			if (i < 0 || i >= maxHeight || j < 0 || j >= maxWidth)
				return;
			
			layers.stream().forEach(l -> {
				l.getImage().setRGB(j, i, Pixel.makeARGB(pix));
			});
		});
		
		imageSaved = projectSaved = false;
		imageChanged = true;
	}

	public Map<String,Operation> getOperations() {
		return operations;
	}
	
	public Operation getOperation(String name) throws ErrorOccurred {
		Operation op = operations.get(name);

		if (op == null)
			throw new ErrorOccurred("Operation with that name doesn't exist!");
		
		return op;
	}

	public void addOperation(Operation op) throws ErrorOccurred {
		if (operations.containsKey(op.getName()))
			throw new ErrorOccurred("Operation with that name already exists!");
		
		operations.put(op.getName(),op);
	}
	
	public void addOperation(String source) throws ErrorOccurred {
		try {
			FileReader file = new FileReader(source);
			BufferedReader reader = new BufferedReader(file);
			addOperation(loadOperation(reader));
			reader.close();
			
		} catch (IOException | NotThat e) {
			throw new ErrorOccurred("Error loading operation!");
		}
	}

	public void saveOperation(String name,String destination) throws ErrorOccurred {
		if (!operations.containsKey(name))
			throw new ErrorOccurred("Operation with that name doesn't exist!");
		
		Operation op = operations.get(name);
		
		try {
			FileWriter fileWriter = new FileWriter(destination);
			XMLFormatter xml = new XMLFormatter();
			fileWriter.write(xml.writeOperation(op));
			fileWriter.close();
			
		} catch (IOException e) {
			throw new ErrorOccurred("Error with writting file!");
		}
	}

	public void saveProject(String destination) throws ErrorOccurred {
		if (!destination.substring(destination.length() - 3).equals("xml"))
			throw new ErrorOccurred("Format not appropriate!");
		
		try {	
			FileWriter file = new FileWriter(destination);
			StringBuilder text = new StringBuilder();
			XMLFormatter xml = new XMLFormatter();
			
			text.append("<Image>\n");
			
			//writing layers
			text.append("<Layers>\n");
			
			for (Layer lay : layers) {
				text.append(xml.writeLayer(lay));
			}
			
			text.append("</Layers>\n");
			//done writing layers
			
			//writing selections
			text.append("<Selections>\n");
			
			selections.values().forEach(s -> {
				text.append(xml.writeSelection(s));
			});
			
			text.append("</Selections>\n");
			//done writing selections
			
			//writing operations
			text.append("<Operations>\n");
			
			operations.values().forEach(o -> {
				text.append(xml.writeOperation(o));
			});
			
			text.append("</Operations>\n");
			//done writting operations
			
			//writing maxHeight and maxWidth
			text.append("<maxHeight>" + maxHeight + "</maxHeight>\n");
			text.append("<maxWidth>" + maxWidth + "</maxWidth>\n");
			//done writing maxHeight and maxWidth
			
			//writing number of active selections
			text.append("<activeSelections>" + activeSelections + "</activeSelections>\n");
			//done writing number of active selections
			
			text.append("</Image>\n");
			
			file.write(text.toString());
			
			projectSaved = true;
			file.close();
			
		} catch (IOException e) {
			throw new ErrorOccurred("Error occurred saving project!");
		}
	}

	public void loadProject(String source) throws ErrorOccurred {
		if (!source.substring(source.length() - 3).equals("xml"))
			throw new ErrorOccurred("Format unappropriate!");
		
		try {
			clearProject();
			FileReader file = new FileReader(source);
			BufferedReader reader = new BufferedReader(file);
			
			String part;
			
			reader.readLine();	// <Image>
			
			part = reader.readLine();	// maybe <Layers>

			if (part.equals("<Layers>")) {
				try {
					while (true)
						loadLayer(reader);
				} catch(NotThat e) {/* </Layers> has been read */}	
			}
			
			part = reader.readLine();	// maybe <Selections>
			
			if (part.equals("<Selections>")) {
				try {
					while (true)
						loadSelection(reader);
				} catch(NotThat e) {/* </Selections> has been read */}	
			}
			
			part = reader.readLine();	// <Operations>
			
			try {
				while (true) {
					Operation op = loadOperation(reader);
					//String name = op.getName();
					//if (!name.equals("Median") && !name.equals("Inverse") && !name.equals("BlackWhite") && !name.equals("Gray"))
					this.addOperation(op);
				}
			} catch(NotThat e) {/* </Operations> has been read */}
			
			part = reader.readLine();	// maxHeight
			
			part = reader.readLine();	// maxWidth
			
			part = reader.readLine();	// activeSelections
			
			part = reader.readLine();	// </Image>
			
			if (!part.equals("</Image>"))
				throw new ErrorOccurred("Error loading project!");	
			
			reader.close();
			
		} catch (IOException e) {
			throw new ErrorOccurred("Error loading project!");
		} 

	}
	
	private void loadLayer(BufferedReader reader) throws IOException,ErrorOccurred , NotThat{
		String line = reader.readLine();	//should be <Layer>
		
		if (!line.equals("<Layer>"))
			throw new NotThat();
		
		Pattern activeP = Pattern.compile("^<active>(.*)</active>$");
		Pattern visibleP = Pattern.compile("^<visible>(.*)</visible>$");
		Pattern transpP = Pattern.compile("^<transparency>(.*)</transparency>$");
		Pattern pathP = Pattern.compile("^<path>(.*)</path>$");
		
		line = reader.readLine();	//active
		Matcher activeM = activeP.matcher(line);
		if (!activeM.matches())
			throw new ErrorOccurred("Error loading layer!");
		int active = Integer.parseInt(activeM.group(1));
		
		line = reader.readLine();	//visible
		Matcher visibleM = visibleP.matcher(line);
		if (!visibleM.matches())
			throw new ErrorOccurred("Error loading layer!");
		int visible = Integer.parseInt(visibleM.group(1));
		
		line = reader.readLine();	//transparency
		Matcher transpM = transpP.matcher(line);
		if (!transpM.matches())
			throw new ErrorOccurred("Error loading layer!");
		int transp = Integer.parseInt(transpM.group(1));
		
		line = reader.readLine();	//image path
		Matcher pathM = pathP.matcher(line);
		if (!pathM.matches())
			throw new ErrorOccurred("Error loading layer!");
		String path = pathM.group(1);
		
		line = reader.readLine();	//</Layer>
		if (!line.equals("</Layer>"))
			throw new ErrorOccurred("Error reading file!");
		
		this.addLayer(path);
		this.setLayerActivity(layers.size() - 1, (active == 1) ? true : false);
		this.setLayerVisibility(layers.size() - 1, (visible == 1) ? true : false);
		this.setLayerTransparency(layers.size() - 1, transp / 100.);
	}

	private void loadSelection(BufferedReader reader) throws IOException,ErrorOccurred , NotThat {
		String line = reader.readLine();	//should be <Selection>
		
		if (!line.equals("<Selection>"))
			throw new NotThat();
		
		Pattern nameP = Pattern.compile("^<name>(.*)</name>$");
		Pattern activeP = Pattern.compile("^<active>(.*)</active>$");
		Pattern pointsP = Pattern.compile("^<Points>(.*)</Points>$");
		Pattern pointP = Pattern.compile("([0-9]*),([0-9]*)");
		
		line = reader.readLine();	//name
		Matcher nameM = nameP.matcher(line);
		if (!nameM.matches())
			throw new ErrorOccurred("Error loading selection!");
		String name = nameM.group(1);
		
		line = reader.readLine();	//active
		Matcher activeM = activeP.matcher(line);
		if (!activeM.matches())
			throw new ErrorOccurred("Error loading selection!");
		int active = Integer.parseInt(activeM.group(1));
		
		
		line = reader.readLine();	//Points
		Matcher pointsM = pointsP.matcher(line);
		if (!pointsM.matches())
			throw new ErrorOccurred("Error loading selection!");
		String points = pointsM.group(1);
		
		line = reader.readLine();	// </Selection>
		if (!line.equals("</Selection>"))
			throw new ErrorOccurred("Error reading selection!");
		
		FileReader pointsFile = new FileReader(points);
		BufferedReader pointsReader = new BufferedReader(pointsFile);
		
		String[] pointsArray = pointsReader.readLine().split(" ");
		Set<Point> pointsSet = new HashSet<Point>();
		
		for (int i = 0; i < pointsArray.length ; i++) {
			Matcher pointM = pointP.matcher(pointsArray[i]);
			if (pointM.matches()) {
				pointsSet.add(new Point(Integer.parseInt(pointM.group(1)),Integer.parseInt(pointM.group(2))));
			}
		}
		
		pointsReader.close();
		
		this.addSelection(name, pointsSet);
		this.setSelectionActivity(name, (active == 1) ? true : false);
	}
	
	private Operation loadOperation(BufferedReader reader) throws IOException,ErrorOccurred , NotThat {
		String line = reader.readLine();	// <Operation>
		
		if (!line.equals("<Operation>"))
			throw new NotThat();
		
		Pattern nameP = Pattern.compile("^<name>(.*)</name>$");
		Pattern sizeP = Pattern.compile("^<size>(.*)</size>$");
		Pattern operationIdP = Pattern.compile("^<operationId>(.*)</operationId>$");
		
		line = reader.readLine();	// name
		Matcher nameM = nameP.matcher(line);
		if (!nameM.matches())
			throw new ErrorOccurred("Error loading operation\n");
		String name = nameM.group(1);
		
		if (name.equals("Median") || name.equals("Inverse") || name.equals("BlackWhite") || name.equals("Gray")) {
			reader.readLine();	// </Operation>
			return new BasicOperation(name);
		}
		
		line = reader.readLine();	//size or operationId
		
		Matcher sizeM = sizeP.matcher(line);
		Matcher operationIdM = operationIdP.matcher(line);
		
		Operation op = null;
		
		if (sizeM.matches()) {	//it is composite operation
			reader.readLine();	// </Operation>
			op = loadCompositeOperation(name,Integer.parseInt(sizeM.group(1)),reader);
			
		} else if (operationIdM.matches()) {	//it is basic operation
			op = loadBasicOperation(name,reader);
			
		} else
			throw new ErrorOccurred("Error loading operation!");
		
		return op;
	}
	
	private Operation loadBasicOperation(String name,BufferedReader reader) throws ErrorOccurred {

		Operation op = null;
		
		try {
			Pattern argumentP = Pattern.compile("^<argument>(.*)</argument>$");
	
			String line = reader.readLine();	//argument, if argument exists
			
			Matcher argumentM = argumentP.matcher(line);
			
			if (argumentM.matches()) {
				
				int arg = Integer.parseInt(argumentM.group(1));
				op = new BasicOperation(name,arg);		
				reader.readLine();	// </Operation>
			} else {
				op = new BasicOperation(name);
			}
			
		} catch (IOException | ErrorOccurred e) {
			throw new ErrorOccurred("Error loading operation!");
		}
		
		return op;
	}
	
	private Operation loadCompositeOperation(String name,int size,BufferedReader reader) throws IOException, ErrorOccurred, NotThat {
		
		List<Operation> operations = new LinkedList<Operation>();
		
		for (int i = 0; i < size ; i++) {
			operations.add(loadOperation(reader));
		}
		
		return new CompositeOperation(name,operations);
	}
	
	public void applyOperation(String name) throws ErrorOccurred{
		
		try {
			this.saveOperation(name, "operationToApply.fun");
			this.saveProject("project.xml");
			
			String file = "data\\C++\\Debug\\POOP - obradaSlike.exe " + "project.xml " + "operationToApply.fun";
			
			Runtime runtime = Runtime.getRuntime();
			
			Process process = runtime.exec(file);
			
			process.waitFor();
			
			this.loadProject("project.xml");
			
			//dodato
			projectSaved = imageSaved = false;
			imageChanged = true;
			
		} catch (IOException | InterruptedException e) {
			throw new ErrorOccurred("Error applying operation!");
		}
	}
}
