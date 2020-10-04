package obradaSlike;

import java.awt.Color;
import java.awt.Dialog;
import java.awt.Frame;
import java.awt.Label;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

@SuppressWarnings("serial")
public class ErrorDialog extends Dialog {

	public ErrorDialog(Frame parent,ErrorOccurred error) {
		super(parent,"Error",false);
		setSize(MainWindow.DIALOG_WIDTH,MainWindow.DIALOG_HEIGHT);
		setBackground(Color.RED);
		Label text = new Label("Error: " + error.getReason());
		text.setAlignment(Label.CENTER);
		add(text);
		setVisible(true);
		
		this.addWindowListener(new WindowAdapter() {
			@Override
			public void windowClosing(WindowEvent arg0) {
				setVisible(false);
			}	
		});
	}
}
