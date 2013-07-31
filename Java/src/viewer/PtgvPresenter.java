package viewer;
import org.lwjgl.opengl.Drawable;
import formats.pgv.TexturedModel;

public class PtgvPresenter extends AnimatedPresenter {
	public PtgvPresenter(Drawable drawable, String file) throws Exception {
		model = new TexturedModel(file);
		numPoses = model.numPoses();
		
		
	}
	
	public void render(int pose) {
		model.setPose(pose);
	}
	
	TexturedModel model;
	int shaders;
}
