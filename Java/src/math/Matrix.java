package math;
import java.io.InputStream;

public class Matrix {
	public int width, height;
	public Matrix(InputStream input) throws Exception {
		height = readInt(input);
		width = readInt(input);
	}
	
	public static float readFloat(final InputStream input) throws Exception {
		return Float.intBitsToFloat(readInt(input));
	}
	
	public static int readInt(final InputStream input) throws Exception{
        final int a = input.read();
        final int b = input.read();
        final int c = input.read();
        final int d = input.read();

        return (( a & 0xff ) << 0) +
        	   (( b & 0xff ) << 8) +
        	   (( c & 0xff ) << 16) +
        	   (( d & 0xff ) << 24);
	}
}
