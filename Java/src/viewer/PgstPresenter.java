package viewer;
import static org.lwjgl.opengl.GL11.*;
import mintools.parameters.DoubleParameter;
import mintools.parameters.IntParameter;

import org.lwjgl.opengl.Drawable;

import formats.pgt.PgstModel;
import formats.pgt.Vertex;

class PgstPresenter extends AnimatedPresenter {
	public PgstPresenter(Drawable drawable, String file) throws Exception {
		model = new PgstModel(file);
		harmonic = new IntParameter("Harmonic", 0, 0, model.numHarmonics() / 3);
		numBands = new IntParameter("Num Bands", model.numBands(), 1, model.numBands());
		geometryRigor = new IntParameter("Geometry Components", 1, 0, model.geometry.rigor);
		exposure = new DoubleParameter("Exposure", 1, 0.01, 5);
		transferRigor = new IntParameter[model.numBands()];
		numPoses = model.numPoses();
		
		for (int i = 0; i < transferRigor.length; i++)
			transferRigor[i] = new IntParameter("Band " + i + " Components", 1, 0, model.bands[i].rigor);
		
		startup(drawable);
		add(harmonic.getSliderControls());
		add(geometryRigor.getSliderControls());
		add(numBands.getSliderControls());
		add(exposure.getSliderControls(false));
	}
	
	public void render(int pose) {
		for (int i = 0; i < transferRigor.length; i++)
			model.bands[i].rigor = transferRigor[i].getValue();
		
		model.geometry.rigor = geometryRigor.getValue();
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
		
	IntParameter[] transferRigor;
	IntParameter harmonic, geometryRigor, numBands;
	DoubleParameter exposure;
	PgstModel model;
}
