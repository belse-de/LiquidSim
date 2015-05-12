
// SVG (Scalable Vector Graphics) file generation
// output things to a .SVG file, for easy input into Adobe Illustrator
/*
<?xml version="1.0" encoding="utf-8"?>
<!-- Generator: Adobe Illustrator 12.0.0, SVG Export Plug-In . SVG Version: 6.00 Build 51448)  -->
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd" [
	<!ENTITY ns_svg "http://www.w3.org/2000/svg">
	<!ENTITY ns_xlink "http://www.w3.org/1999/xlink">
]>
<svg  version="1.1" id="Layer_1" xmlns="&ns_svg;" xmlns:xlink="&ns_xlink;" width="315.19" height="288" viewBox="0 0 315.19 288"
	 overflow="visible" enable-background="new 0 0 315.19 288" xml:space="preserve">
<line fill="none" stroke="#6C9D31" stroke-width="3" x1="72" y1="0" x2="72" y2="288"/>
<line fill="none" stroke="#6C9D31" stroke-width="3" x1="0" y1="72" x2="288" y2="72"/>
<g>
	<g>
		<line fill="none" stroke="#2E3192" stroke-width="3" x1="72" y1="72" x2="126" y2="27"/>
		<path fill="#2E3192" d="M129.475,20.102c-4.77,2.024-8.13,3.028-12.208,4.348l9.688,11.626c0.661-1.432,3.626-6.899,6.479-11.224
			c3.051-4.631,6.047-8.555,8.194-10.854C138.979,15.696,134.579,17.935,129.475,20.102z"/>
	</g>
</g>
<g>
	<g>
		<line fill="none" stroke="#2E3192" stroke-width="3" x1="135" y1="243" x2="297" y2="162"/>
		<path fill="#2E3192" d="M301.962,156.081c-5.106,0.878-8.606,1.085-12.88,1.437l6.769,13.536c0.971-1.242,5.11-5.885,8.877-9.442
			c4.031-3.809,7.846-6.942,10.463-8.688C312.224,153.969,307.427,155.141,301.962,156.081z"/>
	</g>
</g>
</svg>
*/


FILE *p_svg = NULL;

void SVG_Start(const char* p_name)
{
	p_svg = fopen(p_name,"wb");
	fprintf(p_svg,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	fprintf(p_svg,"<!-- Generator: Adobe Illustrator 12.0.0, SVG Export Plug-In . SVG Version: 6.00 Build 51448)  -->\n");
	fprintf(p_svg,"<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\" [\n");
	fprintf(p_svg,"	<!ENTITY ns_svg \"http://www.w3.org/2000/svg\">\n");
	fprintf(p_svg,"	<!ENTITY ns_xlink \"http://www.w3.org/1999/xlink\">\n");
	fprintf(p_svg,"]>\n");
	fprintf(p_svg,"<svg  version=\"1.1\" id=\"Layer_1\" xmlns=\"&ns_svg;\" xmlns:xlink=\"&ns_xlink;\" width=\"315.19\" height=\"288\" viewBox=\"0 0 315.19 288\"\n");
	fprintf(p_svg,"	 overflow=\"visible\" enable-background=\"new 0 0 315.19 288\" xml:space=\"preserve\">\n");
}

void SVG_End()
{
	fprintf(p_svg,"</svg>\n");
	fclose(p_svg);
}

void SVG_Line(unsigned int stroke, float width, float x1, float y1, float x2, float y2)
{
		fprintf(p_svg,"<line fill=\"none\" stroke=\"#%6X\" stroke-width=\"%f\" x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\"/>\n",
			stroke, width, x1,y1,x2,y2);
}

// head w and l are as fraction of the line length
void SVG_Arrow(unsigned int stroke, float width, float x1, float y1, float x2, float y2, float head_l, float head_w)
{
	SVG_Line(stroke,width,x1,y1,x2,y2);
	Vector2 end = Vector2(x2,y2);
	Vector2 line = Vector2(x2,y2) - Vector2(x1,y1);
	Vector2 normal = line.Normal();
	Vector2 side = Vector2(normal.y,-normal.x);
	float len = line.Length();
	Vector2 base = end - head_l * normal * len;
	Vector2 left = base + head_w * side * len; 
	Vector2 right = base - head_w * side * len; 
	SVG_Line(stroke,width,left.x,left.y,end.x,end.y);
	SVG_Line(stroke,width,right.x,right.y,end.x,end.y);

}


void SVG_Dot(unsigned int stroke, float cx, float cy, float r)
{
	fprintf(p_svg,"<circle fill=\"#%6X\" stroke=\"#%6X\" cx=\"%f\" cy=\"%f\" r=\"%f\"/>\n", stroke, stroke, cx,cy,r);
}

void SVG_Test()
{
	SVG_Start("C:\\Mick\\Docs\\GamesIndustry\\GameDeveloperArticles\\Fluid and Gas  Dynamics\\svg_test.svg");
	SVG_Line(0x604323,3,20,20,50,80);
	SVG_End();
}
