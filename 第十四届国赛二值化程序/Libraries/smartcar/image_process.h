#ifndef _IMAGE_PROCESS_H_
#define _IMAGE_PROCESS_H_

void get_route_one(uint8 hang,uint8 start);
void get_route_all(void);
void get_route_endline(void);
void get_route_midline(void);
int regression(int startline,int endline,int p[]);    
void connect(int hang_start,int hang_end,int p[]);
void scratch_line(void);

extern int midline[Row];
extern int leftline[Row];
extern int rightline[Row];
extern int endline;
extern int leftlineflag[Row];
extern int rightlineflag[Row];
extern int track_width[Row];

void GetBlackEndParam(void);

extern int BlackEndLL;
extern int BlackEndM;
extern int BlackEndRR;
extern int scratchflag;

extern int BlackEndMaxMax;
extern int DropRow;

extern int scratch_cnt;

#define hstart 55
#endif 