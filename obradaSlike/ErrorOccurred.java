package obradaSlike;

import java.awt.Dialog;
import java.awt.Frame;

public class ErrorOccurred extends Exception {
	private String reason;

	public ErrorOccurred(String reason) {
		super();
		this.reason = reason;
	}
	
	public String getReason() {
		return reason;
	}
}

class NotThat extends Exception {
	
}