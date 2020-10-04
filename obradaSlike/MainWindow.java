package obradaSlike;

import java.awt.*;

import java.awt.event.*;
import java.awt.image.BufferedImage;
import java.util.LinkedList;
import java.util.Map;
import java.util.stream.Stream;

@SuppressWarnings("serial")
public class MainWindow extends Frame {
	
	public static final int WIDTH = 1000 , HEIGHT = 700;
	public static final int DIALOG_WIDTH = 500 , DIALOG_HEIGHT = 350;
	
	private Canvas canvas = new Canvas();
	
	private Image image = new Image();
	private String selectionName;
	private boolean savingProject;
	private boolean savingImage;
	private boolean loadingProject;
	private boolean applyingOperation;
	private boolean loadingImage;
	
	public MainWindow() {
		super("PooP");
		setSize(WIDTH,HEIGHT);
		setVisible(true);
		add(canvas,BorderLayout.CENTER);
		addMenus();
		addListeners();
	}
	
	public synchronized void savingProject() {
	
		while (savingProject) {
			try {
				wait();
			} catch (InterruptedException e) {}
		}
		
		savingProject = true;
	}
	
	public synchronized void doneSavingProject() {
		savingProject = false;
		notifyAll();
	}
	
	public synchronized void savingImage() {
		
		while (savingImage) {
			try {
				wait();
			} catch (InterruptedException e) {}
		}
		
		savingImage = true;
	}
	
	public synchronized void doneSavingImage() {	
		savingImage = false;
		notifyAll();	
	}
	
	public synchronized void loadingImage() {
		
		while (loadingImage) {
			try {
				wait();
			} catch (InterruptedException e) {}
		}
		
		loadingImage = true;
	}
	
	public synchronized void doneLoadingImage() {	
		loadingImage = false;
		notifyAll();	
	}
	
	public synchronized void loadingProject() {
		while (loadingProject) {
			try {
				wait();
			} catch (InterruptedException e) {}
		}
		
		loadingProject = true;
	}
	
	public synchronized void doneLoadingProject() {
		loadingProject = false;
		notifyAll();
	}
	
	public synchronized void applyingOperation() {
		while (applyingOperation) {
			try {
				wait();
			} catch (InterruptedException e) {}
		}
		
		applyingOperation = true;
	}
	
	public synchronized void doneApplyingOperation() {
		applyingOperation = false;
		notifyAll();
	}
	
	public Image getImage() {
		return image;
	}
	
	public void clearScreen() {
		canvas.getGraphics().clearRect(0, 0, WIDTH, HEIGHT);
	}
	
	private void addMenus() {
		MenuBar bar = new MenuBar();
		
		Menu layers = new Menu("Layers");
		configureLayers(layers);
		
		Menu selections = new Menu("Selections");
		configureSelections(selections);
		
		Menu operations = new Menu("Operations");
		configureOperations(operations);
		
		Menu save = new Menu("Save");
		configureSave(save);
		
		Menu load = new Menu("Load");
		configureLoad(load);
		
		Menu show = new Menu("Show");
		configureShow(show);
		
		bar.add(load);
		bar.add(save);
		bar.add(layers);
		bar.add(selections);
		bar.add(operations);
		bar.add(show);
		
		this.setMenuBar(bar);
	}
	
	private void configureLoad(Menu menu) {
		menu.add("Project");
		menu.add("Operation");
		
		menu.addActionListener(e -> {
			if (e.getActionCommand().equals("Project")) {
				new LoadProjectDialog();
			} else {
				new LoadOperationDialog();
			}
		});
	}

	private void configureSave(Menu menu) {
		menu.add("Image");
		menu.add("Project");
		menu.add("Operation");
		
		menu.addActionListener(e -> {
			switch (e.getActionCommand()) {
			case "Image":
				new SaveImageDialog();
				break;
			case "Project":
				new SaveProjectDialog();
				break;
			case "Operation":
				new SaveOperationDialog();
				break;
			}
		});
	}

	private void configureSelections(Menu menu) {
		menu.add("New");
		menu.add("Delete");
		menu.add("Colour");
		
		menu.addActionListener(e -> {
			if (e.getActionCommand().equals("New")) {			
				new NewSelectionTutorialDialog();
				NewSelection ns = new NewSelection();
				MainWindow.this.canvas.addKeyListener(ns);
				MainWindow.this.canvas.addMouseListener(ns);	
				
			} else if (e.getActionCommand().equals("Delete")) {
				new DeleteSelectionDialog();
			} else {
				new ColourSelectionDialog();
			}
		});
	}
	
	private void configureLayers(Menu menu) {	
		menu.add("New");
		menu.add("Delete");
		
		menu.addActionListener(e -> {
			if (e.getActionCommand().equals("New")) {
				new NewLayerDialog();
			} else {
				new DeleteLayerDialog();
			}
		});
	}
	
	private void configureOperations(Menu menu) {
		menu.add("New");
		menu.add("Apply");
		
		menu.addActionListener(e -> {
			if (e.getActionCommand().equals("New")) {
				new NewOperationDialog();
			} else {
				new ApplyOperationDialog();
			}
		});
	}

	private void configureShow(Menu show) {
		show.add("Image");
		show.add("Layers");
		show.add("Selections");
		show.add("Operations");
		
		show.addActionListener(e -> {
			switch(e.getActionCommand()) {
			case "Image":
				showImage();
				break;
			case "Layers":
				new ShowLayersDialog();
				break;
			case "Selections":
				new ShowSelectionsDialog();
				break;
			case "Operations":
				new ShowOperationDialog();
				break;
			}
		});
	}
	
	private void addListeners() {
		//when user press x button
		this.addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				dispose();
			}
		});
	}
	
	public void showImage() {
		image.updateImage();
		canvas.getGraphics().clearRect(0, 0, WIDTH, HEIGHT);
		//canvas.getGraphics().drawImage(image.getImage(), 0, 0, WIDTH, HEIGHT, null);
		canvas.getGraphics().drawImage(image.getImage(), 0, 0, null);
	}

	private class NewLayerDialog extends Dialog {
	
		public NewLayerDialog() {
			super(MainWindow.this,"New layer",false);
			setSize(DIALOG_WIDTH,DIALOG_HEIGHT);
			addComponents();
			setVisible(true);
			
			this.addWindowListener(new WindowAdapter() {
				@Override
				public void windowClosing(WindowEvent arg0) {
					setVisible(false);
				}			
			});
		}

		private void addComponents() {
			this.setLayout(new GridLayout(4,1));
			
			Panel wh = new Panel();
			wh.add(new Label("Height:"));
			TextField widthTextField = new TextField(3);
			wh.add(widthTextField);
			wh.add(new Label("Width:"));
			TextField heightTextField = new TextField(3);
			wh.add(heightTextField);
			this.add(wh);
			
			Panel path = new Panel();
			path.add(new Label("Image path:"));
			TextField pathTextField = new TextField(45); 
			path.add(pathTextField);
			this.add(path);
			
			CheckboxGroup cbg = new CheckboxGroup();
			Checkbox cbwh = new Checkbox("Use height and width",cbg,false);
			Checkbox cbp = new Checkbox("Use image path",cbg,false);
			
			Panel checkBoxes = new Panel(new GridLayout(2,1));
			checkBoxes.add(cbwh);
			checkBoxes.add(cbp);
			this.add(checkBoxes);
			
			Button confirm = new Button("Confirm");
			confirm.setBackground(Color.GREEN);
			this.add(confirm);
			
			confirm.addActionListener(e -> {		
				try {
					if (cbwh.getState()) {	//width and height checkbox is selected
						image.addLayer(Integer.parseInt(heightTextField.getText()), Integer.parseInt(widthTextField.getText()));
						
					} else if (cbp.getState()) {	//image path checkbox is selected
						//image.addLayer(pathTextField.getText());
						new LoadImageThread(pathTextField.getText()).start();
						
					} else {		//nothing is selected
						image.addLayer();
					}
				} catch(ErrorOccurred err) { 
					new ErrorDialog(MainWindow.this,err);
				}
				
				setVisible(false);
			});
			
			cbwh.addItemListener(e -> {
				if (e.getStateChange() == ItemEvent.SELECTED) {
					pathTextField.setEnabled(false);
					widthTextField.setEnabled(true);
					heightTextField.setEnabled(true);
				}
			});
			
			cbp.addItemListener(e -> {
				if (e.getStateChange() == ItemEvent.SELECTED) {
					pathTextField.setEnabled(true);
					widthTextField.setEnabled(false);
					heightTextField.setEnabled(false);
				}
			});
		}
	}

	private class DeleteLayerDialog extends Dialog {

		public DeleteLayerDialog() {
			super(MainWindow.this,"Delete layer",false);
			setSize(MainWindow.DIALOG_WIDTH,MainWindow.DIALOG_HEIGHT);
			this.setLayout(new GridLayout(2,1));	
			addComponents();
			setVisible(true);
			
			this.addWindowListener(new WindowAdapter() {
				@Override
				public void windowClosing(WindowEvent arg0) {
					setVisible(false);
				}			
			});
		
		}
		
		private void addComponents() {
			Panel up = new Panel();
			up.add(new Label("Layer's number:"));
			TextField number = new TextField(3);
			up.add(number);
			
			this.add(up);
			
			Button confirm = new Button("Confirm");
			confirm.setBackground(Color.GREEN);
			this.add(confirm);
			
			confirm.addActionListener(e -> {
				try {
					image.removeLayer(Integer.parseInt(number.getText()));
				} catch (NumberFormatException e1) {
					new ErrorDialog(MainWindow.this,new ErrorOccurred("Wrong number format!"));
				} catch (ErrorOccurred e1) {
					new ErrorDialog(MainWindow.this,e1);
				}
				setVisible(false);
			});
		}
	}

	private class NewOperationDialog extends Dialog {
		
		java.util.List<Operation> operations = new java.util.LinkedList<Operation>();
		
		public NewOperationDialog() {
			super(MainWindow.this,"New operation",false);
			setSize(MainWindow.DIALOG_WIDTH,MainWindow.DIALOG_HEIGHT);
			addComponents();
			setVisible(true);
			
			this.addWindowListener(new WindowAdapter() {
				@Override
				public void windowClosing(WindowEvent arg0) {
					setVisible(false);
				}			
			});
		}

		private void addComponents() {
			this.setLayout(new GridLayout(1,2,50,0));
			
			//adding left panel
			Panel left = new Panel(new GridLayout(13,1));
			CheckboxGroup checkboxGroup = new CheckboxGroup();
			Checkbox checkboxes[] = new Checkbox[12];
			
			checkboxes[0] = new Checkbox("Addition",checkboxGroup,false);
			checkboxes[1] = new Checkbox("Subtraction",checkboxGroup,false);
			checkboxes[2] = new Checkbox("Reverse Subtraction",checkboxGroup,false);
			checkboxes[3] = new Checkbox("Multiplication",checkboxGroup,false);
			checkboxes[4] = new Checkbox("Division",checkboxGroup,false);
			checkboxes[5] = new Checkbox("Reverse Division",checkboxGroup,false);
			checkboxes[6] = new Checkbox("Power",checkboxGroup,false);
			checkboxes[7] = new Checkbox("Logarithm",checkboxGroup,false);
			checkboxes[8] = new Checkbox("Absolute value",checkboxGroup,false);
			checkboxes[9] = new Checkbox("Minimum",checkboxGroup,false);
			checkboxes[10] = new Checkbox("Maximum",checkboxGroup,false);
			checkboxes[11] = new Checkbox("Other",checkboxGroup,false);
			
			for (int i = 0; i < 11 ; i++) {	//adding basic operations
				left.add(checkboxes[i]);
			}
			
			TextField operationName = new TextField(10);
			Panel otherOperationPanel = new Panel(new GridLayout(1,0));
			otherOperationPanel.add(checkboxes[11]);
			otherOperationPanel.add(operationName);
			left.add(otherOperationPanel);
			
			TextField argument = new TextField(5);
			Panel argumentPanel = new Panel(new GridLayout(1,0));
			argumentPanel.setBackground(Color.YELLOW);
			argumentPanel.add(new Label("Argument:"));
			argumentPanel.add(argument);
			left.add(argumentPanel);
			
			this.add(left);
			
			//adding right panel
			Panel right = new Panel(new GridLayout(2,1,0,40));
			
			List operationList = new List(5);
			
			right.add(operationList);
			
			Panel buttonsPanel = new Panel(new GridLayout(2,1,0,15));
			Button add = new Button("Add");
			add.setBackground(Color.BLUE);
			Button confirm = new Button("Confirm");
			confirm.setBackground(Color.GREEN);
			buttonsPanel.add(add);
			buttonsPanel.add(confirm);
			
			right.add(buttonsPanel);
			
			this.add(right);
			
			//adding listeners
			add.addActionListener(e -> {
				for (int i = 0; i < 12;i++) {
					if (checkboxes[i].getState()) {
						String text = checkboxes[i].getLabel();
						String argText = argument.getText();
						
						try {
							if (text.equals("Absolute value") || text.equals("Logarithm")) {
								operations.add(new BasicOperation(text));
								operationList.add(text);
								break;
							}	
						
							if (i == 11) {	//its other predefined or already existing function
								operations.add(MainWindow.this.image.getOperation(operationName.getText()));
								operationList.add(operationName.getText());
								break;
							}
							
							if (argText.equals(""))
								throw new ErrorOccurred("Argument not inserted!");
							
							int arg = Integer.parseInt(argText);
							
							operations.add(new BasicOperation(text,arg));
							operationList.add(text + " " + arg);
							break;
							
						} catch(ErrorOccurred err) {
							new ErrorDialog(MainWindow.this,err);
							break;
						}
						
					}
				}
			});
			
			confirm.addActionListener(e -> {
				
				new Dialog(MainWindow.this, "Operation name", false) {
					public void start() {
						setSize(MainWindow.DIALOG_WIDTH ,MainWindow.DIALOG_HEIGHT );
						setVisible(true);
						addComponents();
						
						this.addWindowListener(new WindowAdapter() {
							@Override
							public void windowClosing(WindowEvent arg0) {
								setVisible(false);
							}			
						});
					}
					
					private void addComponents() {
						this.setLayout(new GridLayout(3,1,0,50));
						
						this.add(new Label("Operation name:"));
						
						TextField operationName = new TextField(10);
						
						this.add(operationName);
						
						Button confirm = new Button("Confirm");
						confirm.setBackground(Color.GREEN);
						
						this.add(confirm);
						
						//now adding listener
						confirm.addActionListener(e -> {
							if (operationName.getText().equals("")) {
								new ErrorDialog(MainWindow.this,new ErrorOccurred("Operation name not inserted!"));
								return;
							}
							
							try {
								MainWindow.this.image.addOperation(new CompositeOperation(operationName.getText(),operations));							
								setVisible(false);
							} catch (ErrorOccurred e1) {
								new ErrorDialog(MainWindow.this,e1);
							}
							
						});
					}
				}.start();
				
				NewOperationDialog.this.setVisible(false);
			});
			
			for (int i = 0; i < 11 ; i++) {
				if (i == 8 || i == 7) {
					checkboxes[i].addItemListener(e -> {
						if (e.getStateChange() == ItemEvent.SELECTED) {
							argument.setEnabled(false);
							operationName.setEnabled(false);
						}
					});
					continue;
				}
				checkboxes[i].addItemListener(e -> {
					if (e.getStateChange() == ItemEvent.SELECTED) {
						argument.setEnabled(true);
						operationName.setEnabled(false);
					}
				});
			}
			
			checkboxes[11].addItemListener(e -> {
				if (e.getStateChange() == ItemEvent.SELECTED) {
					argument.setEnabled(false);
					operationName.setEnabled(true);
				}
			});
		}
	}

	private class NewSelectionTutorialDialog extends Dialog {

		public NewSelectionTutorialDialog() {
			super(MainWindow.this,"New selection tutorial",false);
			this.setSize(MainWindow.DIALOG_WIDTH,MainWindow.DIALOG_HEIGHT);
			this.setVisible(true);
			this.setLayout(new GridLayout(3,1));
			
			Label info = new Label("Press enter when you are done creating rectangles");
			info.setAlignment(Label.CENTER);
			info.setFont(new Font(null,Font.BOLD,15));
		
			Button confirm = new Button("Confirm");
			confirm.setBackground(Color.GREEN);
			
			TextField selectName = new TextField(10);
			
			Panel selectNamePanel = new Panel();
			selectNamePanel.add(new Label("Selection name"));
			selectNamePanel.add(selectName);
			
			this.add(info);
			this.add(selectNamePanel);
			this.add(confirm);
			
			showImage();
			
			confirm.addActionListener(e -> {
				MainWindow.this.selectionName = selectName.getText();
				setVisible(false);
			});
			
			this.addWindowListener(new WindowAdapter() {
				@Override
				public void windowClosing(WindowEvent arg0) {
					setVisible(false);
				}			
			});
		}
		
	}
	
	private class NewSelection extends MouseAdapter implements KeyListener {

		private java.util.List<Rectangle> rectangles = new LinkedList<Rectangle>();
		private Point start,end;

		private void addRectangle(Point start,Point end) {
			int w = Math.abs(start.getX() - end.getX());
			int h = Math.abs(start.getY() - end.getY());
			
			Point upLeft = null;
			
			if (start.getX() < end.getX() && start.getY() < end.getY())
				upLeft = start;
			
			if (start.getX() > end.getX() && start.getY() > end.getY())
				upLeft =  end;
			
			if (start.getX() < end.getX() && start.getY() > end.getY())
				upLeft =  start.addY(-h);
			
			if (start.getX() > end.getX() && start.getY() < end.getY())
				upLeft =  start.addX(-w);
			
			if (upLeft == null)
				return; 
			
			rectangles.add(new Rectangle(upLeft,w,h));
			
			drawDashedRectangle(canvas.getGraphics(), upLeft, w, h);
		}
		
		private void drawDashedRectangle(Graphics g,Point upLeft,int w,int h) {
			//creates a copy of the Graphics instance
	        Graphics2D g2d = (Graphics2D) g.create();

	        //set the stroke of the copy, not the original 
	        Stroke dashed = new BasicStroke(3, BasicStroke.CAP_BUTT, BasicStroke.JOIN_BEVEL, 0, new float[]{9}, 0);
	        g2d.setStroke(dashed);
	        
	        g2d.drawLine(upLeft.getX(),upLeft.getY() , upLeft.addX(w).getX(),upLeft.addX(w).getY());
	        g2d.drawLine(upLeft.getX(),upLeft.getY() , upLeft.addY(h).getX(),upLeft.addY(h).getY());
	        g2d.drawLine(upLeft.addX(w).getX(),upLeft.addX(w).getY() , upLeft.addX(w).addY(h).getX(),upLeft.addX(w).addY(h).getY());
	        g2d.drawLine(upLeft.addY(h).getX(),upLeft.addY(h).getY() , upLeft.addX(w).addY(h).getX(),upLeft.addX(w).addY(h).getY());

	        //gets rid of the copy
	        g2d.dispose();
		}

		@Override
		public void mousePressed(MouseEvent arg) {
			start = new Point(arg.getX(),arg.getY());
		}

		@Override
		public void mouseReleased(MouseEvent arg) {
			end = new Point(arg.getX(),arg.getY());
			addRectangle(start,end);
		}

		@Override
		public void keyPressed(KeyEvent arg) {
			if (arg.getKeyCode() == KeyEvent.VK_ENTER) {
				MainWindow.this.image.addSelection(selectionName, rectangles);
				MainWindow.this.canvas.removeKeyListener(this);
				MainWindow.this.canvas.removeMouseListener(this);
				MainWindow.this.clearScreen();
			}
		}

		@Override
		public void keyReleased(KeyEvent arg0) {}

		@Override
		public void keyTyped(KeyEvent arg0) {}
		
	}

	private class DeleteSelectionDialog extends Dialog {
		
		public DeleteSelectionDialog() {
			super(MainWindow.this,"Delete selection",false);
			setSize(MainWindow.DIALOG_WIDTH,MainWindow.DIALOG_HEIGHT);
			addComponents();
			setVisible(true);
			
			this.addWindowListener(new WindowAdapter() {
				@Override
				public void windowClosing(WindowEvent arg0) {
					setVisible(false);
				}			
			});
		}
		
		private void addComponents() {
			this.setLayout(new GridLayout(2,1));
			
			Panel upPanel = new Panel();
			upPanel.add(new Label("Selection's name:"));
			TextField selectName = new TextField(10);
			upPanel.add(selectName);
			
			Button confirm = new Button("Confirm");
			confirm.setBackground(Color.GREEN);
			
			this.add(upPanel);
			this.add(confirm);
			
			confirm.addActionListener(e->{
				try {
					MainWindow.this.image.removeSelection(selectName.getText());
				} catch (ErrorOccurred e1) {
					new ErrorDialog(MainWindow.this,e1);
				}
				setVisible(false);
			});
		}
	}

	private class ShowSelectionsDialog extends Dialog {
		
		public ShowSelectionsDialog() {
			super(MainWindow.this,"Selections",false);
			setSize(MainWindow.DIALOG_WIDTH,MainWindow.DIALOG_HEIGHT);
			addComponents();
			setVisible(true);
			
			this.addWindowListener(new WindowAdapter() {
				@Override
				public void windowClosing(WindowEvent arg0) {
					setVisible(false);
				}			
			});
		}
		
		private void addComponents() {
			this.setLayout(new GridLayout(3,1,0,50));	
			
			Choice selectionsList = new Choice();
			
			Map<String,Selection> selections = MainWindow.this.image.getSelections();
			selections.keySet().stream().forEach(s -> selectionsList.add(s));
			
			Label selectionsLabel = new Label("Selection:");
			selectionsLabel.setAlignment(Label.CENTER);
			selectionsLabel.setFont(new Font(null,Font.BOLD,15));
			
			Checkbox checkbox = new Checkbox("Active");
			checkbox.setSize(50, 50);
			
			Panel downPanel = new Panel(new GridLayout(1,2));
			downPanel.add(checkbox);
			
			Button apply = new Button("Apply");
			apply.setBackground(Color.GREEN);
			downPanel.add(apply);
			
			this.add(selectionsLabel);
			this.add(selectionsList);
			this.add(downPanel);
				
			//adding listeners
			apply.addActionListener(e -> {
				if (selectionsList.getItemCount() == 0)
					return;
				
				try {
					image.setSelectionActivity(selectionsList.getSelectedItem(), checkbox.getState());
				} catch (ErrorOccurred e1) {}
	
			});
		}
	}
	
	private class ShowLayersDialog extends Dialog {
		
		public ShowLayersDialog() {
			super(MainWindow.this,"Show layers",false);
			setSize(MainWindow.DIALOG_WIDTH,MainWindow.DIALOG_HEIGHT);
			addComponents();
			setVisible(true);
			
			this.addWindowListener(new WindowAdapter() {
				@Override
				public void windowClosing(WindowEvent arg0) {
					setVisible(false);
				}
			});
		}
		
		private void addComponents() {
			this.setLayout(new GridLayout(2,1,0,40));
			
			Panel upPanel = new Panel(new BorderLayout(0,20));
			
			Label layerLabel = new Label("Layer:");
			layerLabel.setAlignment(Label.CENTER);
			layerLabel.setFont(new Font(null,Font.BOLD,15));
			upPanel.add(layerLabel,BorderLayout.NORTH);
			
			Choice layerChoice = new Choice();
			java.util.List<Layer> layers = MainWindow.this.image.getLayers();
			for (int i = 0; i < layers.size() ; i++)
				layerChoice.add("Layer number: " + i);
			
			upPanel.add(layerChoice,BorderLayout.CENTER);
			
			Panel downPanel = new Panel(new GridLayout(1,2,40,0));
			
			Panel selectPanel = new Panel(new GridLayout(3,1,0,20));
			Checkbox visibleCheck = new Checkbox("Visible",false);
			Checkbox activeCheck = new Checkbox("Active",false);
			Panel transpPanel = new Panel(new FlowLayout(FlowLayout.LEFT));
			TextField transpText = new TextField(3);
			transpPanel.add(transpText);
			transpPanel.add(new Label("Transparency [0-1]"));
			selectPanel.add(visibleCheck);
			selectPanel.add(activeCheck);
			selectPanel.add(transpPanel);
			
			downPanel.add(selectPanel);
			
			Button apply = new Button("Apply");
			apply.setBackground(Color.GREEN);
			
			downPanel.add(apply);
			
			this.add(upPanel);
			this.add(downPanel);
			
			//adding listener
			apply.addActionListener(e -> {
				if (layerChoice.getItemCount() == 0)
					return;
				
				if (transpText.getText().equals("")) {
					new ErrorDialog(MainWindow.this,new ErrorOccurred("Transparency not inserted!"));
					return;
				}
				
				int index = layerChoice.getSelectedIndex();
				
				boolean active = activeCheck.getState();
				boolean visible = visibleCheck.getState();
				double transp = Double.parseDouble(transpText.getText());
				
				Layer lay = layers.get(index);
				try {
					image.setLayerActivity(index, active);
					image.setLayerVisibility(index, visible);
					image.setLayerTransparency(index, transp);			
				} catch(ErrorOccurred e1) {}
				
				showImage();
			});
		}
	}

	private class ShowOperationDialog extends Dialog {
		
		public ShowOperationDialog() {
			super(MainWindow.this,"Show operations",false);
			setSize(MainWindow.DIALOG_WIDTH,MainWindow.DIALOG_HEIGHT);
			addComponents();
			setVisible(true);
			
			this.addWindowListener(new WindowAdapter() {
				@Override
				public void windowClosing(WindowEvent arg0) {
					setVisible(false);
				}			
			});
		}
		
		private void addComponents() {
			Label operationsLabel = new Label("Operations:");
			operationsLabel.setAlignment(Label.CENTER);
			operationsLabel.setFont(new Font(null,Font.BOLD,10));
			
			List operationsList = new List(10);
			image.getOperations().keySet().stream().forEach(o -> {
				operationsList.add(o);
			});
			
			this.add(operationsLabel,BorderLayout.NORTH);
			this.add(operationsList,BorderLayout.CENTER);
		}
	}

	private class SaveImageDialog extends Dialog {
		
		public SaveImageDialog() {
			super(MainWindow.this,"Save image",false);
			setSize(MainWindow.DIALOG_WIDTH,MainWindow.DIALOG_HEIGHT);
			addComponents();
			setVisible(true);
			
			this.addWindowListener(new WindowAdapter() {
				@Override
				public void windowClosing(WindowEvent arg0) {
					setVisible(false);
				}			
			});
		}
		
		private void addComponents() {
			this.setLayout(new GridLayout(2,1));
			
			Panel upPanel = new Panel(new FlowLayout(FlowLayout.CENTER));
			Label labela = new Label("Destination:");
			labela.setAlignment(Label.CENTER);
			labela.setFont(new Font(null,Font.BOLD,15));
			TextField dest = new TextField(10);
			
			upPanel.add(labela);
			upPanel.add(dest);
			
			Button save = new Button("Save");
			save.setBackground(Color.GREEN);
			
			this.add(upPanel);
			this.add(save);
			
			save.addActionListener(e -> {
				new SaveImageThread(dest.getText()).start();
				setVisible(false);
			});
		}
	}

	private class SaveOperationDialog extends Dialog {
		
		public SaveOperationDialog() {
			super(MainWindow.this,"Save operation",false);
			setSize(MainWindow.DIALOG_WIDTH,MainWindow.DIALOG_HEIGHT);
			addComponents();
			setVisible(true);
			
			this.addWindowListener(new WindowAdapter() {
				@Override
				public void windowClosing(WindowEvent arg0) {
					setVisible(false);
				}			
			});
		}
		
		private void addComponents() {
			this.setLayout(new GridLayout(3,1,0,40));
			
			Panel panel1 = new Panel(new FlowLayout(FlowLayout.LEFT));
			panel1.add(new Label("Name:         "));
			TextField opName = new TextField(10);
			panel1.add(opName);
			
			Panel panel2 = new Panel(new FlowLayout(FlowLayout.LEFT));
			panel2.add(new Label("Destination:"));
			TextField dest = new TextField(10);
			panel2.add(dest);
			
			Button save = new Button("Save");
			save.setBackground(Color.GREEN);
			
			this.add(panel1);
			this.add(panel2);
			this.add(save);
			
			//add listener
			save.addActionListener(e -> {
				try {
					MainWindow.this.image.saveOperation(opName.getText(), dest.getText());
					new SuccessDialog("Operation successfully saved!");
				} catch (ErrorOccurred e1) {
					new ErrorDialog(MainWindow.this,e1);
				}
				
				setVisible(false);
			});
		}
	}

	private class SaveProjectDialog extends Dialog {
		
		public SaveProjectDialog() {
			super(MainWindow.this,"Save project",false);
			setSize(MainWindow.DIALOG_WIDTH,MainWindow.DIALOG_HEIGHT);
			addComponents();
			setVisible(true);
			
			this.addWindowListener(new WindowAdapter() {
				@Override
				public void windowClosing(WindowEvent arg0) {
					setVisible(false);
				}			
			});
		}
		
		private void addComponents() {
			this.setLayout(new GridLayout(2,1));
			
			Panel upPanel = new Panel(new FlowLayout(FlowLayout.CENTER));
			Label labela = new Label("Destination:");
			labela.setAlignment(Label.CENTER);
			labela.setFont(new Font(null,Font.BOLD,15));
			TextField dest = new TextField(10);
			
			upPanel.add(labela);
			upPanel.add(dest);
			
			Button save = new Button("Save");
			save.setBackground(Color.GREEN);
			
			this.add(upPanel);
			this.add(save);
			
			save.addActionListener(e -> {
				new SaveProjectThread(dest.getText()).start();
				setVisible(false);
			});
		}
	}

	private class SaveProjectThread extends Thread {
		
		private String destination;
		
		public SaveProjectThread(String destination) {
			this.destination = destination;
		}
		
		@Override
		public void run() {
			MainWindow.this.savingProject();
			
			try {
				MainWindow.this.image.saveProject(destination);
				if (!applyingOperation)
					new SuccessDialog("Project successfully saved!");
			} catch (ErrorOccurred e) {
				new ErrorDialog(MainWindow.this,e);
			}
			
			MainWindow.this.doneSavingProject();
		}	
	}
	
	private class SaveImageThread extends Thread {
		
		private String destination;
		
		public SaveImageThread(String destination) {
			this.destination = destination;
		}
		
		@Override
		public void run() {
			MainWindow.this.savingImage();
			
			try {
				MainWindow.this.image.saveImage(destination);
				new SuccessDialog("Image successfully saved!");
			} catch (ErrorOccurred e) {
				new ErrorDialog(MainWindow.this,e);
			}
			
			MainWindow.this.doneSavingImage();
		}	
	}

	private class SuccessDialog extends Dialog {
		
		public SuccessDialog(String reason) {
			super(MainWindow.this,"Success",false);
			setSize(MainWindow.DIALOG_WIDTH,MainWindow.DIALOG_HEIGHT);
			
			this.setBackground(Color.GREEN);
			Label label = new Label(reason);
			label.setFont(new Font(null,Font.BOLD,25));
			label.setAlignment(Label.CENTER);		
			this.add(label);
			
			setVisible(true);
			
			this.addWindowListener(new WindowAdapter() {
				@Override
				public void windowClosing(WindowEvent arg0) {
					setVisible(false);
				}			
			});
		}
	}

	private class LoadProjectDialog extends Dialog {
		public LoadProjectDialog() {
			super(MainWindow.this,"Load project",false);
			setSize(MainWindow.DIALOG_WIDTH,MainWindow.DIALOG_HEIGHT);
			addComponents();
			setVisible(true);
			
			this.addWindowListener(new WindowAdapter() {
				@Override
				public void windowClosing(WindowEvent arg0) {
					setVisible(false);
				}			
			});
		}
		
		private void addComponents() {
			this.setLayout(new GridLayout(2,1));
			
			Panel upPanel = new Panel(new FlowLayout(FlowLayout.CENTER));
			Label labela = new Label("Source:");
			labela.setAlignment(Label.CENTER);
			labela.setFont(new Font(null,Font.BOLD,15));
			TextField src = new TextField(10);
			
			upPanel.add(labela);
			upPanel.add(src);
			
			Button load = new Button("Load");
			load.setBackground(Color.GREEN);
			
			this.add(upPanel);
			this.add(load);
			
			load.addActionListener(e -> {
				new LoadProjectThread(src.getText()).start();
				setVisible(false);
			});
		}
	}

	private class LoadProjectThread extends Thread {
		
		private String source;
		
		public LoadProjectThread(String source) {
			this.source = source;
		}
		
		@Override
		public void run() {
			MainWindow.this.loadingProject();
			
			try {
				MainWindow.this.image.loadProject(source);
				if (!applyingOperation)
					new SuccessDialog("Project successfully loaded!");
			} catch (ErrorOccurred e) {
				new ErrorDialog(MainWindow.this,e);
			}
			
			MainWindow.this.doneLoadingProject();
		}	
	}
	
	private class LoadOperationDialog extends Dialog {
		
		public LoadOperationDialog() {
			super(MainWindow.this,"Load operation",false);
			setSize(MainWindow.DIALOG_WIDTH,MainWindow.DIALOG_HEIGHT);
			addComponents();
			setVisible(true);
			
			this.addWindowListener(new WindowAdapter() {
				@Override
				public void windowClosing(WindowEvent arg0) {
					setVisible(false);
				}			
			});
		}
		
		private void addComponents() {
			this.setLayout(new GridLayout(2,1));
			
			Panel upPanel = new Panel(new FlowLayout(FlowLayout.CENTER));
			Label labela = new Label("Source:");
			labela.setAlignment(Label.CENTER);
			labela.setFont(new Font(null,Font.BOLD,15));
			TextField src = new TextField(10);
			
			upPanel.add(labela);
			upPanel.add(src);
			
			Button load = new Button("Load");
			load.setBackground(Color.GREEN);
			
			this.add(upPanel);
			this.add(load);
			
			load.addActionListener(e -> {
				try {
					MainWindow.this.image.addOperation(src.getText());
					new SuccessDialog("Operation successfully loaded!");
				} catch (ErrorOccurred e1) {
					new ErrorDialog(MainWindow.this, e1);
				}
				setVisible(false);
			});
		}
	}

	private class ApplyOperationDialog extends Dialog {
	
		public ApplyOperationDialog() {
			super(MainWindow.this,"Apply operation",false);
			setSize(MainWindow.DIALOG_WIDTH,MainWindow.DIALOG_HEIGHT);
			addComponents();
			setVisible(true);
			
			this.addWindowListener(new WindowAdapter() {
				@Override
				public void windowClosing(WindowEvent arg0) {
					setVisible(false);
				}			
			});
		}
		
		private void addComponents() {
			this.setLayout(new GridLayout(2,1));
			
			Panel upPanel = new Panel(new FlowLayout(FlowLayout.CENTER));
			Label labela = new Label("Operation name:");
			labela.setAlignment(Label.CENTER);
			labela.setFont(new Font(null,Font.BOLD,15));
			TextField opName = new TextField(10);
			
			upPanel.add(labela);
			upPanel.add(opName);
			
			Button apply = new Button("Apply");
			apply.setBackground(Color.GREEN);
			
			this.add(upPanel);
			this.add(apply);
			
			apply.addActionListener(e -> {
				new ApplyOperationThread(opName.getText()).start();
				setVisible(false);
			});
		}
	}
	
	private class ApplyOperationThread extends Thread{
		
		private String name;
		
		public ApplyOperationThread(String name) {
			this.name = name;
		}
		
		@Override
		public void run() {
			MainWindow.this.applyingOperation();
			
			try {
				MainWindow.this.image.applyOperation(name);
				new SuccessDialog("Operation successfully applied!");
			} catch (ErrorOccurred e) {
				new ErrorDialog(MainWindow.this,e);
			}
			MainWindow.this.doneApplyingOperation();
		}	
	}
	
	private class LoadImageThread extends Thread {
		
		private String source;
		
		public LoadImageThread(String source) {
			this.source = source;
		}
		
		@Override
		public void run() {
			MainWindow.this.loadingImage();
			
			try {
				MainWindow.this.image.addLayer(source);
				new SuccessDialog("Image successfully loaded!");
			} catch (ErrorOccurred e) {
				new ErrorDialog(MainWindow.this,e);
			}
			
			MainWindow.this.doneLoadingImage();
		}	
	}
	
	private class ColourSelectionDialog extends Dialog {
		
		public ColourSelectionDialog() {
			super(MainWindow.this,"Colour selection",false);
			setSize(MainWindow.DIALOG_WIDTH,MainWindow.DIALOG_HEIGHT);
			addComponents();
			setVisible(true);
			
			this.addWindowListener(new WindowAdapter() {
				@Override
				public void windowClosing(WindowEvent arg0) {
					setVisible(false);
				}			
			});
		}
		
		private void addComponents() {
			this.setLayout(new GridLayout(3, 1, 0, 30));
			
			Label label = new Label("Selection name:");
			label.setAlignment(Label.CENTER);
			label.setFont(new Font(null,Font.BOLD,20));
			
			TextField selName = new TextField(20);
			
			Panel topPanel = new Panel(new BorderLayout());
			topPanel.add(label,BorderLayout.NORTH);
			topPanel.add(selName,BorderLayout.CENTER);
			
			TextField red = new TextField(5);
			TextField green = new TextField(5);
			TextField blue = new TextField(5);
			
			Label rLabel = new Label("R:");
			rLabel.setFont(new Font(null,Font.BOLD,20));
			Label gLabel = new Label("G:");
			gLabel.setFont(new Font(null,Font.BOLD,20));
			Label bLabel = new Label("B:");
			bLabel.setFont(new Font(null,Font.BOLD,20));
			
			Panel rPanel = new Panel();
			rPanel.setBackground(Color.RED);
			rPanel.add(rLabel);
			rPanel.add(red);
			
			Panel gPanel = new Panel();
			gPanel.setBackground(Color.GREEN);
			gPanel.add(gLabel);
			gPanel.add(green);
			
			Panel bPanel = new Panel();
			bPanel.setBackground(Color.BLUE);
			bPanel.add(bLabel);
			bPanel.add(blue);
			
			Panel middlePanel = new Panel(new GridLayout(1,3));
			middlePanel.add(rPanel);
			middlePanel.add(gPanel);
			middlePanel.add(bPanel);
			
			Button confirm = new Button("Confirm");
			confirm.setBackground(Color.GREEN);
			
			this.add(topPanel);
			this.add(middlePanel);
			this.add(confirm);
			
			confirm.addActionListener(e -> {
				int r = Integer.parseInt(red.getText());
				int g = Integer.parseInt(green.getText());
				int b = Integer.parseInt(blue.getText());
				String name = selName.getText();
				
				try {
					MainWindow.this.image.colourSelection(name, r, g, b);
				} catch (ErrorOccurred e1) {
					new ErrorDialog(MainWindow.this, e1);
				}
				
				setVisible(false);
			});
		}
	}
}
