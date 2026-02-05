import sqlite3

DDL = """
CREATE TABLE IF NOT EXISTS CuttingProcessData (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    CODE TEXT,
    ON1 REAL,
    OFF1 REAL,
    IPM REAL,
    HP REAL,
    MA REAL,
    SV REAL,
    V REAL,
    SF REAL,
    WS REAL,
    WT REAL,
    W_MATERIAL TEXT,
    W_THICK REAL,
    D_DIAMETER REAL,
    D_MATERIAL TEXT,
    GAP REAL,
    CUTSPEED REAL,
    RA REAL,
    LAG REAL,
    remark TEXT
);
"""

def ensure_table():
    conn = connect()
    cur = conn.cursor()
    cur.execute(DDL)
    conn.commit()
    conn.close()

DB_PATH = "/home/shj/database/date/wedm.db"

FIELDS = [
    ("CODE", str),
    ("ON1", float),
    ("OFF1", float),
    ("IPM", float),
    ("HP", float),
    ("MA", float),
    ("SV", float),
    ("V", float),
    ("SF", float),
    ("WS", float),
    ("WT", float),
    ("W_MATERIAL", str),
    ("W_THICK", float),
    ("D_DIAMETER", float),
    ("D_MATERIAL", str),
    ("GAP", float),
    ("CUTSPEED", float),
    ("RA", float),
    ("LAG", float),
    ("remark", str),
]


def connect():
    return sqlite3.connect(DB_PATH)


def show_all():
    conn = connect()
    cur = conn.cursor()
    cur.execute("SELECT id, CODE, CUTSPEED, RA, LAG, remark FROM CuttingProcessData")
    rows = cur.fetchall()
    conn.close()

    print("\n--- 当前数据库数据 ---")
    for r in rows:
        print(f"ID={r[0]:3d} | CODE={r[1]:6s} | V={r[2]} | Ra={r[3]} | LAG={r[4]} | {r[5]}")
    print("----------------------\n")


def show_one():
    id_ = input("输入 ID：")
    conn = connect()
    cur = conn.cursor()
    cur.execute("SELECT * FROM CuttingProcessData WHERE id=?", (id_,))
    row = cur.fetchone()
    conn.close()

    if not row:
        print("❌ 未找到该 ID")
        return

    print("\n--- 详细数据 ---")
    for i, col in enumerate([d[0] for d in cur.description]):
        print(f"{col:12s}: {row[i]}")
    print("----------------\n")


def input_value(name, tp):
    val = input(f"{name}（回车跳过）：")
    if val == "":
        return None
    try:
        return tp(val)
    except:
        print("⚠️ 输入类型错误，已置为空")
        return None


def insert_row():
    print("\n请输入工艺参数：")
    values = []
    cols = []

    for name, tp in FIELDS:
        v = input_value(name, tp)
        if v is not None:
            cols.append(name)
            values.append(v)

    if not cols:
        print("❌ 未输入任何数据，取消")
        return

    sql = f"""
    INSERT INTO CuttingProcessData ({",".join(cols)})
    VALUES ({",".join("?" for _ in cols)})
    """

    conn = connect()
    cur = conn.cursor()
    cur.execute(sql, values)
    conn.commit()
    conn.close()

    print("✅ 数据已插入\n")


def update_remark():
    id_ = input("输入 ID：")
    remark = input("新的备注：")

    conn = connect()
    cur = conn.cursor()
    cur.execute(
        "UPDATE CuttingProcessData SET remark=? WHERE id=?",
        (remark, id_)
    )
    conn.commit()
    conn.close()

    print("✅ 备注已更新\n")


def delete_row():
    id_ = input("输入要删除的 ID：")
    conn = connect()
    cur = conn.cursor()
    cur.execute("DELETE FROM CuttingProcessData WHERE id=?", (id_,))
    conn.commit()
    conn.close()
    print("🗑 已删除\n")


def main():
    ensure_table()
    while True:
        print("""
==============================
 Cutting Process DB Manager
==============================
1. 查看全部数据
2. 按 ID 查看一条
3. 新增一条工艺数据
4. 修改备注
5. 删除一条数据
0. 退出
""")
        cmd = input("请选择操作：")

        if cmd == "1":
            show_all()
        elif cmd == "2":
            show_one()
        elif cmd == "3":
            insert_row()
        elif cmd == "4":
            update_remark()
        elif cmd == "5":
            delete_row()
        elif cmd == "0":
            break
        else:
            print("❓ 无效输入")


if __name__ == "__main__":
    main()
