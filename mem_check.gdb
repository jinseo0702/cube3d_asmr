# 먼저 프로그램을 실행합니다
run ./map/map3.cub

# 프로그램이 실행되면 모니터링 루프를 시작합니다
define memory_monitor
  set pagination off
  while 1
    shell echo -n "Memory usage at: " && date
    info proc mappings
    shell echo "Process memory info:"
    shell ps u | head -1; ps u | grep cub3D
    continue
    shell sleep 2
  end
end

# 프로그램이 실행 중이면 중단(Ctrl+C)하고 memory_monitor 명령어를 입력하세요
