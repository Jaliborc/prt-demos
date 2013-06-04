package graphics;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

public class Buffers {
	public static FloatBuffer allocateFloats(int count) {
		ByteBuffer bytes = ByteBuffer.allocateDirect(count * 4);
	    bytes.order(ByteOrder.nativeOrder());
	    
	    return bytes.asFloatBuffer();
	}
}
