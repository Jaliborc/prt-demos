package math;
import java.io.ObjectInputStream;

public class Svd {
	public int rigor;
	public Svd(ObjectInputStream input) throws Exception {
		m = (float[]) input.readObject();
		v = (float[][]) input.readObject();
		u = (float[][]) input.readObject();
		rigor = u.length;
	}
	
	public float[] get(int i) {
		float[] vector = m.clone();
		
		for (int y = 0; y < v.length; y++)
			for (int x = 0; x < rigor; x++)
				vector[y] += v[y][x] * u[x][i];

		return vector;
	}
	
	public int height() {
		return v.length;
	}
	
	public int width() {
		return u[0].length;
	}
	
	float[] m;
	float[][] v, u;
}
