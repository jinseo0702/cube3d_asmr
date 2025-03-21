# ifndef STRUCT_H
# define STRUCT_H


# define TEX_WIDTH 64
# define TEX_HEIGHT 64

typedef struct s_allimg
{
	void					*img;   
    char					*buffer;    
	int						width;      
	int						height;     
    int						pixel_bits; 
	int						line_bytes; 
    int						endian;     
}			t_allimg;

typedef struct s_wall_tex
{
	char	*buffer;    
	int		line_bytes; 
	int		pixel_bits; 
	int		tex_x;      
	int		height;     
}			t_wall_tex;

typedef struct s_two_coordi_node
{
    double x;      
    double y;      
    char c;
    double dir;    
}               t_two_coordi_node;

typedef struct s_draw_func
{
    int dx;
    int dy;
    int sx;
    int sy;
    int err;
    int e2;
}               t_draw_func;

typedef struct s_map
{
    int fd;
    int high;
    int map_width;
    int map_height;
    int exf;
    char *NO;
    char *SO;
    char *WE;
    char *EA;
    int F;
    int C;
    char **map;
}   t_map;

typedef struct s_ray
{
    double dir_x;        
    double dir_y;        
    
    int map_x;           
    int map_y;           
    
    int step_x;          
    int step_y;          
    
    double side_dist_x;  
    double side_dist_y;  
    
    double delta_dist_x; 
    double delta_dist_y; 
    
    int hit;             
    int side;            
    
    double perp_wall_dist; 
} t_ray;

typedef struct  s_data 
{
    void    *mlx;       
    void    *win;       
    int						width;      
	int						height;     
    double fov;
    t_map   map;
    t_allimg   img;            
    t_two_coordi_node cor;
    t_ray ray;
    t_allimg tex_n;     
    t_allimg tex_s;     
    t_allimg tex_w;     
    t_allimg tex_e;     
    int status;
}               t_data;

#endif