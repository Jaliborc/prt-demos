package math;
import java.io.InputStream;

public class FloatMatrix extends Matrix {
	public float[] values;
	
	public FloatMatrix(InputStream input) throws Exception {
		super(input);
		values = new float[height * width];
		
		for (int i = 0; i < values.length; i++)
			values[i] = readFloat(input);
	}
	
	public float get(int x, int y) {
		return values[x * height + y];
	}

	public float get(int i) {
		return values[i];
	}
}
