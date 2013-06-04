package math;
import java.io.InputStream;

public class IntMatrix extends Matrix {
	public int[] values;
	
	public IntMatrix(InputStream input) throws Exception {
		super(input);
		values = new int[height * width];
		
		for (int i = 0; i < values.length; i++)
			values[i] = readInt(input);
	}
	
	public int get(int x, int y) {
		return values[x * height + y];
	}
}
