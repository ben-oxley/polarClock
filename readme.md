polarClock
============

polarClock is a simple polar clock with an outer ring of seconds, then minutes and hours on the inside.

This script is based on some of the Pebble Watch example scripts and builds on the principles.

![watchFace](http://benoxley.co.uk/wp-content/uploads/2014/01/2014-01-07-003.png)

Coding
------------

The main code is very simple and uses a main call block to draw circles around the central point.

~~~cpp
void line_layer_update_callback(Layer *layer, GContext* ctx) {
  struct tm * timenow;
  time_t timestamp = time(NULL);
  timenow = localtime(&timestamp);

  graphics_context_set_fill_color(ctx, GColorWhite);
  APP_LOG(APP_LOG_LEVEL_INFO,"%d %d %d",timenow->tm_hour,timenow->tm_min,timenow->tm_sec);
  //draw hours
  int i;
  for (i = 0; i < 12*15; i++) {
    if(abs((timenow->tm_hour%12)-(i/15)) > 1) {
      graphics_fill_circle(ctx,get_gpoint(i*2,35),4);
      
    }
  }
  for (i = 0; i < 60*6; i++) {
    if(abs(timenow->tm_min-(i/6)) > 5) {
      graphics_fill_circle(ctx,get_gpoint(i,47),4);
      
    }
  }
  for (i = 0; i < 60*6; i++) {
    if(abs(timenow->tm_sec-(i/6)) > 5) {
      graphics_fill_circle(ctx,get_gpoint(i,60),4);
      
    }
  }

}
~~~

And a very simple polar to cartesian co-ordinate conversion

~~~cpp
GPoint get_gpoint(int degrees,int radius){
  double x = degrees, y = degrees;
  GPoint a;
  x = radius*sin_lookup((x/360.0)*TRIG_MAX_ANGLE);
  x /= TRIG_MAX_RATIO;
  y = -radius*cos_lookup((y/360.0)*TRIG_MAX_ANGLE);
  y /= TRIG_MAX_RATIO;
  a = GPoint((int)x+72,(int)y+74);
  //APP_LOG(APP_LOG_LEVEL_INFO,"%d %d",(int)x,(int)y);
  return a;
}
~~~

![watch](http://benoxley.co.uk/wp-content/uploads/2014/01/2014-01-06-23.57.20-768x1024.jpg)

It's always possible that some extra information may be found at: [benoxley.com](http://benoxley.com/ "benoxley.com")