package obradaSlike;

public class Pixel {

	private int red;
	private int green;
	private int blue;
	private int alpha;
	
	public Pixel(int red , int green , int blue , int alpha) {
		this.red = red;
		this.green = green;
		this.blue = blue;
		this.alpha = alpha;
	}
	
	public Pixel() {
		this(255,255,255,255);
	}
	
	public Pixel(int red , int green , int blue) {
		this(red,green,blue,255);
	}
	
	public int getRed() {
		return red;
	}
	public void setRed(int red) {
		this.red = red;
	}
	public int getGreen() {
		return green;
	}
	public void setGreen(int green) {
		this.green = green;
	}
	public int getBlue() {
		return blue;
	}
	public void setBlue(int blue) {
		this.blue = blue;
	}
	public int getAlpha() {
		return alpha;
	}
	public void setAlpha(int alpha) {
		this.alpha = alpha;
	}
	
	public static Pixel makePixel(int argb) {
		return new Pixel((argb >> 16) & 0xff,(argb >> 8) & 0xff,argb & 0xff,(argb >> 24) & 0xff);
	}
	
	public static int makeARGB(Pixel pix) {
		return (pix.getAlpha() << 24) | (pix.getRed() << 16) | (pix.getGreen() << 8) | pix.getBlue();
	}
}
