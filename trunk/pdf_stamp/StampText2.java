
import java.io.FileOutputStream;
import java.io.IOException;

import com.itextpdf.text.DocumentException;
import com.itextpdf.text.Element;
import com.itextpdf.text.Phrase;
import com.itextpdf.text.pdf.ColumnText;
import com.itextpdf.text.pdf.PdfContentByte;
import com.itextpdf.text.pdf.PdfReader;
import com.itextpdf.text.pdf.PdfStamper;
import com.itextpdf.text.BaseColor;
import com.itextpdf.text.Rectangle;

public class StampText2 {
    
    public static void main(String[] args)
        throws DocumentException, IOException {
        stamp(args[0], args[1], args[2], args[3], args[4]);
        
    }

	// options: text, point, rectangle, 
    public static void stamp(String src, String dest, String text, String len, String len2)
        throws IOException, DocumentException {
        PdfReader reader = new PdfReader(src);
		Rectangle size = reader.getPageSize(1); //72 pixels per inch
		double w = size.getRight();
		double h = size.getTop();
		//System.out.println("right" + size.getRight() + " -- " + size.getRight() / 72);
		//System.out.println("top" + size.getTop() + " -- " + size.getTop() / 72);
        PdfStamper stamper = new PdfStamper(reader, new FileOutputStream(dest));
        PdfContentByte canvas = stamper.getOverContent(1);
        ColumnText.showTextAligned(canvas,
                Element.ALIGN_LEFT, new Phrase(text), (int)(w * (0.905 -Double.parseDouble(len2))), (int)(h * 0.085), 0);
		canvas.saveState();
		canvas.setColorStroke(BaseColor.BLACK);
		int defaultLen = (int)(w * 0.07);
		int lenInt = (int)(w * Double.parseDouble(len));
		canvas.rectangle((int)(w * (0.90 -Double.parseDouble(len2))), (int)(h * 0.08), lenInt, (int)(h * 0.025));
		canvas.stroke();
		canvas.saveState();
        stamper.close();
    }

}
