__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE|CLK_ADDRESS_CLAMP|CLK_FILTER_NEAREST;

__kernel void bilinear_interpolation(__read_only image2d_t input_image, __write_only image2d_t output_image, const float x_ratio, const float y_ratio)
{
	int i = get_global_id(0);
	int j = get_global_id(1);

	int x0 = floor(x_ratio * j);
	int y0 = floor(y_ratio * i);
	int x1 = ceil(x_ratio * j);
	int y1 = ceil(y_ratio * i);

	double xw = (x_ratio * j) - x0;
	double yw = (y_ratio * i) - y0;
	
	float4 a = read_imagef(input_image, sampler, (int2) (y0, x0));
	float4 b = read_imagef(input_image, sampler, (int2) (y0, x1));
	float4 c = read_imagef(input_image, sampler, (int2) (y1, x0));
	float4 d = read_imagef(input_image, sampler, (int2) (y1, x1));

	float4 a0 = {
		a.x * (1 - xw) * (1 - yw),
		a.y * (1 - xw) * (1 - yw),
		a.z * (1 - xw) * (1 - yw),
		a.w * (1 - xw) * (1 - yw)
	};

	float4 b0 = {
		b.x * xw * (1 - yw), 
		b.y * xw * (1 - yw), 
		b.z * xw * (1 - yw),
		b.w * xw * (1 - yw)
	};

	float4 c0 = {
		c.x * yw * (1 - xw),
		c.y * yw * (1 - xw), 
		c.z * yw * (1 - xw),
		c.w * yw * (1 - xw)
	};
	
	float4 d0 = {
		d.x * xw * yw,
		d.y * xw * yw,
		d.z * xw * yw,
		d.w * xw * yw
	};

	float4 output_color = {
		a0.x + b0.x + c0.x + d0.x,
		a0.y + b0.y + c0.y + d0.y,
		a0.z + b0.z + c0.z + d0.z,
		a0.w + b0.w + c0.w + d0.w
	};

	write_imagef(output_image, (int2)(i, j), output_color);
}