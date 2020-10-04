package obradaSlike;

public class BasicOperation extends Operation {

	enum OperationsIds { ADD, SUB, REV_SUB, MUL, DIV, REV_DIV, POW, LOG, ABS, MIN, MAX,PREDEFINED };
	
	private final OperationsIds id;
	private final int argument;	//if it has Integer.MAX_VALUE , then operation has no argument
	
	public BasicOperation(String name,int argument) throws ErrorOccurred {
		super(name);
		this.argument = argument;
		
		switch (name) {
		case "Addition":
			id = OperationsIds.ADD;
			break;
		case "Subtraction":
			id = OperationsIds.SUB;
			break;
		case "Reverse Subtraction":
			id = OperationsIds.REV_SUB;
			break;
		case "Multiplication":
			id = OperationsIds.MUL;
			break;
		case "Division":
			id = OperationsIds.DIV;
			break;
		case "Reverse Division":
			id = OperationsIds.REV_DIV;
			break;
		case "Power":
			id = OperationsIds.POW;
			break;
		case "Logarithm":
			id = OperationsIds.LOG;
			break;
		case "Absolute value":
			id = OperationsIds.ABS;
			break;
		case "Minimum":
			id = OperationsIds.MIN;
			break;
		case "Maximum":
			id = OperationsIds.MAX;
			break;
		case "Inverse": case "Gray" : case "BlackWhite" : case "Median":
			id = OperationsIds.PREDEFINED;
			break;
		default:
			throw new ErrorOccurred("Operation with that name doesn't exist!");	
		}
	}
	
	public BasicOperation(String name) throws ErrorOccurred {
		this(name,Integer.MAX_VALUE);
	}
	
	@Override
	public String writeXML() {
		String text = "";
		
		text += "<Operation>\n";
		text += "<name>" + name + "</name>\n";
		
		if (id != OperationsIds.PREDEFINED)
			text += "<operationId>" + id.ordinal() + "</operationId>\n";
		
		if (hasArgument())
			text += "<argument>" + argument + "</argument>\n";
		
		text += "</Operation>\n";
		
		return text;
	}

	public boolean hasArgument() {
		return argument != Integer.MAX_VALUE;
	}
}
