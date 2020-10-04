package obradaSlike;

import java.util.List;

public class CompositeOperation extends Operation {

	private List<Operation> operations;
	
	public CompositeOperation(String name,List<Operation> operations) {
		super(name);
		this.operations = operations;
	}

	@Override
	public String writeXML() {
		String text = "";

		text += "<Operation>\n";

		text += "<name>" + name + "</name>\n";

		text += "<size>" + operations.size() + "</size>\n";

		text += "</Operation>\n";
		
		for (Operation op : operations)
			text += op.writeXML();

		return text;
	}

	public boolean hasArgument() {
		return false;
	}
}
