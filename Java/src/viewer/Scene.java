package viewer;
import static org.lwjgl.opengl.GL13.*;
import static java.lang.Float.parseFloat;
import static org.lwjgl.util.glu.GLU.*;
import static org.lwjgl.opengl.GL11.*;
import org.lwjgl.util.glu.Sphere;

import org.newdawn.slick.opengl.TextureLoader;
import org.newdawn.slick.util.ResourceLoader;
import org.newdawn.slick.opengl.Texture;

import java.io.InputStreamReader;
import java.io.FileInputStream;
import java.io.BufferedReader;
import java.nio.FloatBuffer;

class Scene extends Sphere {
	public float radiance[];
	
	public Scene(String path) throws Exception {
		String extension = path.substring(path.lastIndexOf('.') + 1);
		String radiancePath = path.substring(0, path.lastIndexOf('.')) + ".radiance";
		String radianceSource = new BufferedReader(new InputStreamReader(new FileInputStream(radiancePath))).readLine();
		String[] values = radianceSource.split(" ");
		
		texture = TextureLoader.getTexture(extension, ResourceLoader.getResourceAsStream(path));
		radiance = new float[values.length];
		
		for (int i = 0; i < values.length; i++)
			radiance[i] = parseFloat(values[i]);
		
		setOrientation(GLU_INSIDE);
		setTextureFlag(true);
	}
	
	public void draw(double exposure) {
		glActiveTexture(GL_TEXTURE0);
		texture.bind();
		
		glColor3d(exposure, exposure, exposure);
		glPushMatrix();
		glRotatef(180f, 0,1,0);
		glRotatef(-90f, 0,0,1);
		draw(50, 50, 50);
		glPopMatrix();
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	
	Texture texture;
}
