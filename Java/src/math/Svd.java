package math;
import java.io.InputStream;

public class Svd {
	public int rigor;
	public Svd(InputStream input) throws Exception {
		m = new FloatMatrix(input);
		v = new FloatMatrix(input);
		u = new FloatMatrix(input);
		rigor = u.height;
	}
	
	public float[] get(int i) {
		float[] vector = m.values.clone();
		
		for (int y = 0; y < v.height; y++)
			for (int x = 0; x < rigor; x++)
				vector[y] += v.get(x, y) * u.get(i, x);

		return vector;
	}
	
	public int height() {
		return v.height;
	}
	
	public int width() {
		return u.width;
	}
	
	FloatMatrix m, v, u;
}
