package obradaSlike;

import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class Selection {

	private String name;
	private boolean active = true;
	private Set<Point> collection = new HashSet<Point>();
	
	public Selection(String name,List<Rectangle> rectangles) {
		this.name = name;
		
		for (Rectangle rect : rectangles) {
			rect.getAllPoints().stream().forEach(e -> collection.add(e));
		}
	}
	
	public Selection(String name,Set<Point> points) {
		this.name = name;
		collection = points;
	}
	
	public boolean isActive() {
		return active;
	}
	
	public void setActive(boolean active) {
		this.active = active;
	}
	
	public String getName() {
		return name;
	}
	
	public Set<Point> getAllPoints() {
		return collection;
	}
	
	public boolean contains(Point p) {
		return collection.contains(p);
	}
}
