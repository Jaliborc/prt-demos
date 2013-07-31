package viewer;
import static org.lwjgl.opengl.GL11.*;
import org.lwjgl.opengl.Drawable;

import mintools.parameters.DoubleParameter;
import mintools.parameters.IntParameter;

import formats.pgt.Model;
import formats.pgt.JointModel;
import formats.pgt.Vertex;

class PgtPresenter extends AnimatedPresenter {
	public PgtPresenter(Drawable drawable, String file) throws Exception {
		model = new JointModel(file);
		harmonic = new IntParameter("Harmonic", 0, 0, model.numHarmonics() / 3);
		geometryRigor = new IntParameter("Geometry Components", 1, 0, model.geometry.rigor);
		transferRigor = new IntParameter("Transfer Components", 1, 0, model.transfer.rigor);
		numBands = new IntParameter("Num Bands", model.numBands(), 1, model.numBands());
		exposure = new DoubleParameter("Exposure", 1, 0.01, 5);
		numPoses = model.numPoses();
		
		startup(drawable);
		add(harmonic.getSliderControls());
		add(geometryRigor.getSliderControls());
		add(transferRigor.getSliderControls());
		add(numBands.getSliderControls());
		add(exposure.getSliderControls(false));
	}
	
	public void render(int pose) {
		model.geometry.rigor = geometryRigor.getValue();
		model.transfer.rigor = transferRigor.getValue();
		model.render(pose, harmonic.getValue(), scene.radiance);
	}
	
	public void draw() {
		super.draw();
		glBegin(GL_TRIANGLES);
		
		float e = exposure.getFloatValue();
		for (int index : model.faces) {
			Vertex vertex = model.vertices[index];
			glColor3f(vertex.r * e, vertex.g * e, vertex.b * e);
			glVertex3f(vertex.x, vertex.y, vertex.z);
		}
		
		glEnd();
	}
	
	IntParameter harmonic, geometryRigor, transferRigor, numBands;
	DoubleParameter exposure;
	JointModel model;
}
