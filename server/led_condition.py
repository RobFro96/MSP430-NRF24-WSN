import dataclasses
import enum
import logging
import sqlite3


class LedColor(enum.IntEnum):
    RED = (1 << 0)
    GREEN = (1 << 1)
    BLUE = (1 << 2)


class LedConditionType(enum.IntEnum):
    ALL = 0
    ANY = 1


@dataclasses.dataclass
class LedCondition:
    node_addr: int
    led_color: int
    condition: str
    interval: int  # in sec
    condition_type: LedConditionType

    def get_query_result(self, database: sqlite3.Connection):
        result = database.execute("""
        SELECT condition, count(*)
        FROM (
            SELECT CASE WHEN %(condition)s THEN 1 ELSE 0 END AS condition
            FROM Data
            WHERE time >= strftime('%%s', 'now') - %(interval)d AND addr = %(addr)d
        )
        GROUP BY condition""" % dict(
            addr=self.node_addr, condition=self.condition, interval=self.interval)).fetchall()

        logging.debug("%r > %r", self, result)

        if len(result) == 0:
            return False

        falseCount = self.get_condition_count(result, 0)
        trueCount = self.get_condition_count(result, 1)

        if self.condition_type == LedConditionType.ALL:
            return falseCount == 0 and trueCount > 0
        elif self.condition_type == LedConditionType.ANY:
            return trueCount > 0
        return False

    @classmethod
    def get_condition_count(cls, query, condition):
        for cond, count in query:
            if cond == condition:
                return count
        return 0
