package obradaSlike;

public class Point {
	
	private int x;
	private int y;
	
	public Point(int x, int y) {
		this.x = x;
		this.y = y;
	}
	
	public Point() {
		this(0,0);
	}

	public int getX() {
		return x;
	}

	public int getY() {
		return y;
	}

	public Point addX(int xx) {
		return new Point(x + xx,y);
	}
	
	public Point addY(int yy) {
		return new Point(x,y + yy);
	}
	
	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		
		if (!(obj instanceof Point))
			return false;
		
		Point p2 = (Point)obj;
		
		return x == p2.x && y == p2.y;
	}
	
	
}
