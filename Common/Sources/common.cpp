#include "common.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "Time/time.h"
#include <list>
#include <stdarg.h>
const int MaxQueueSize = 10;
struct Message
{
  string message;
  bool status;
};
list<Message> q;
const int messageLen = 200, timeLen = 10;
char messageBuf[messageLen], timeBuf[timeLen];

void debug_common(const char *fmt, int status, va_list args)
{
  vsnprintf(messageBuf, messageLen, fmt, args);
  snprintf(timeBuf, timeLen, "[%.2f] ", Time::time());
  
  if (q.size() >= MaxQueueSize)
  {
    q.pop_front();
  }

  q.push_back({string(timeBuf) + string(messageBuf), (bool)status});

  printf("%s%s\n", timeBuf, messageBuf);

}
void debug_error(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  debug_common(fmt, 0, args);
  va_end(args);
}
void debug_log(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  debug_common(fmt, 1, args);
  va_end(args);
}
void debug_show()
{
  for (const Message &m: q)
    ImGui::TextColored(m.status ? ImVec4(1,1,1,1) : ImVec4(1,0.1f,0.1f,1) , "%s", m.message.c_str());
  
}