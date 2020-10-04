package obradaSlike;

import java.util.LinkedList;
import java.util.List;

public class Rectangle {

	private Point upperLeftPoint;
	private int width;
	private int height;
	
	public Rectangle(Point upperLeftPoint, int width, int height) {
		super();
		this.upperLeftPoint = upperLeftPoint;
		this.width = width;
		this.height = height;
	}
	
	public Point getUpperLeftPoint() {
		return upperLeftPoint;
	}
	public void setUpperLeftPoint(Point upperLeftPoint) {
		this.upperLeftPoint = upperLeftPoint;
	}
	public int getWidth() {
		return width;
	}
	public void setWidth(int width) {
		this.width = width;
	}
	public int getHeight() {
		return height;
	}
	public void setHeight(int height) {
		this.height = height;
	}
	
	List<Point> getAllPoints() {
		List<Point> points = new LinkedList<Point>();
		
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				points.add(upperLeftPoint.addY(i).addX(j));

		return points;
	}
	
	public boolean contains(Point p) {
		boolean xd = (p.getX() >= upperLeftPoint.getX()) && (p.getX() <= upperLeftPoint.addX(width).getX());
		boolean yd = (p.getY() >= upperLeftPoint.getY()) && (p.getY() <= upperLeftPoint.addY(height).getY());

		return xd && yd;
	}
}
