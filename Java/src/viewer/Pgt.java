package viewer;
import static java.lang.System.currentTimeMillis;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL20.glUseProgram;

import org.lwjgl.LWJGLException;
import org.lwjgl.opengl.Drawable;

import mintools.parameters.BooleanParameter;
import mintools.parameters.DoubleParameter;
import mintools.parameters.IntParameter;
import formats.pgt.Model;
import formats.pgt.PgtModel;
import formats.pgt.PgstModel;
import formats.pgt.Vertex;
import graphics.FileSelector;
import graphics.Presenter;

public class Pgt extends Presenter {
	public Pgt(final Drawable drawable, String file) throws Exception {
		model = file.endsWith(".pgst") ? new PgstModel(file) : new PgtModel(file);
		scene = new Scene(sceneFile);
		
		loop = new BooleanParameter("Loop", false);
		drawScene = new BooleanParameter("Draw Scene", true);
		pose =  new IntParameter("Pose", 0, 0, model.numPoses() - 1);
		harmonic = new IntParameter("Harmonic", 0, 0, model.numHarmonics() / 3);
		//geometryRigor = new IntParameter("Geometry Components", 1, 0, model.geometry.rigor);
		//transferRigor = new IntParameter("Transfer Components", 1, 0, model.transfer.rigor);
		numBands = new IntParameter("Num Bands", model.numBands(), 1, model.numBands());
		exposure = new DoubleParameter("Exposure", 1, 0.01, 5);
		
		loadScene();
		add(new FileSelector("Scene", "../Ambients/", "tga", "png") {
			protected void selected(String file) {
				try {
					if (!drawable.isCurrent())
						drawable.makeCurrent();
					
					sceneFile = file;
					loadScene();
				} catch (Exception e) {e.printStackTrace();}
			}
		});
		
		add(loop.getControls());
		add(drawScene.getControls());
		add(pose.getSliderControls());
		add(harmonic.getSliderControls());
		//add(geometryRigor.getSliderControls());
		//add(transferRigor.getSliderControls());
		add(numBands.getSliderControls());
		add(exposure.getSliderControls(false));
	}
	
	public void draw() throws Exception {
		if (sceneFile != null)
			loadScene();
		
		float e = exposure.getFloatValue();
		if (drawScene.getValue())
			scene.draw(e);
		
		long now = currentTimeMillis();
		if (now > loopTime) {
			int currentPose = pose.getValue() + (loop.getValue() ? 1 : 0);
			if (currentPose == model.numPoses())
				currentPose = 0;
			
			loopTime = now + 34;
			//model.geometry.rigor = geometryRigor.getValue();
			//model.transfer.rigor = transferRigor.getValue();
			model.render(currentPose, harmonic.getValue(), scene.radiance);
			pose.setValue(currentPose);
		}
		
		glUseProgram(0);
		glBegin(GL_TRIANGLES);
		
		for (int index : model.faces) {
			Vertex vertex = model.vertices[index];
			glColor3f(vertex.r * e, vertex.g * e, vertex.b * e);
			glVertex3f(vertex.x, vertex.y, vertex.z);
		}
		
		glEnd();
	}
	
	void loadScene() throws Exception {
		scene = new Scene(sceneFile);
		sceneFile = null;
	}
	
	Model model; Scene scene;
	String sceneFile = "../Ambients/CornellBox.tga";
	
	DoubleParameter exposure;
	BooleanParameter loop, drawScene;
	IntParameter pose, harmonic, geometryRigor, transferRigor, numBands;
	
	long loopTime;
}
