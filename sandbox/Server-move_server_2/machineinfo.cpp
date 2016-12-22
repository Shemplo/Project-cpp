#include "machineinfo.h"

MachineInfo::MachineInfo () {
	Machine ms1;
	ms1.name   = "MS-1";
	ms1.id     = 1;
	ms1.level  = 1;
	ms1.health = 140;
	ms1.armor  = 14;
	ms1.speed  = 4;
	ms1.rotate = 3;
	ms1.damage = 20;
	ms1.duration = 60 / 10;
	ms1.bullet = 11;
	ms1.capacity = 50;
	machines.insert (ms1.id, ms1);
	
	Machine t1;
	t1.name   = "T-1";
	t1.id     = 2;
	t1.level  = 1;
	t1.health = 150;
	t1.armor  = 7;
	t1.speed  = 3;
	t1.rotate = 3.5;
	t1.damage = 5;
	t1.duration = 60 / 40;
	t1.bullet = 13;
	t1.capacity = 150;
	machines.insert (t1.id, t1);
}

MachineInfo::Machine MachineInfo::getInfo (qint32 id) {
	if (machines.contains (id)) {
		return machines.value (id);
	}
	
	return Machine ();
}
