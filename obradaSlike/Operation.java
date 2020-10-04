package obradaSlike;

public abstract class Operation {
	
	protected String name;
	
	public Operation(String name) {
		this.name = name;
	}
	
	public String getName() {
		return name;
	}
	
	public abstract String writeXML();
	
	public abstract boolean hasArgument();
}
