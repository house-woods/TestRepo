#include <stdio.h>
#include <stdlib.h>

typedef struct pixel_type
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} pixel_t;

int main(int argc, char* argv[])
{
	FILE* old_image;
	old_image = fopen(argv[1],"r");

	if (old_image == NULL) {
		fprintf(stderr,"files opened fail\n");
		exit(1);
	};

	char letter_type;
	fscanf(old_image,"%c",&letter_type);
	int number_type;
	fscanf(old_image,"%d",&number_type);
	int width;
	fscanf(old_image,"%d",&width);
	int height;
	fscanf(old_image,"%d",&height);
	int max;
	fscanf(old_image,"%d",&max);

	FILE* new_image = fopen(argv[2],"w");

	if(new_image == NULL) {
		return(-1);
	}

	fprintf(new_image,"%c",letter_type);
	fprintf(new_image,"%d\n",number_type);
	fprintf(new_image,"%d ",width);
	fprintf(new_image,"%d ",height);
	fprintf(new_image,"%d\n",max);

	pixel_t pixel[height][width];
	int i,j;
	int y,z;
	y = height;
    z = 0;

	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			fscanf(old_image,"%c%c%c",&pixel[i][j].r,&pixel[i][j].g,&pixel[i][j].b);
		}
	}
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			int temp = pixel[i][j].r;
			int temp_one = pixel[i][j].g;
			int temp_two = pixel[i][j].b;
			pixel[i][j].r = pixel[y][z].r;
			pixel[i][j].g = pixel[y][z].g;
			pixel[i][j].b = pixel[y][z].b;
			pixel[y][z].r = temp;
			pixel[y][z].g = temp_one;
			pixel[y][z].b = temp_two;
			y--;
			z++;
		}
	}
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			fprintf(new_image,"%c%c%c",pixel[y][z].r,pixel[y][z].g,pixel[y][z].b);
		}
	}
	return 0;
}
