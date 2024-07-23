<template>
  <div>
    <el-row :gutter="20">
      <el-col :span="6" v-if="searched != ''">
        <el-card
          class="box-card"
          @click.native="showDetail(searched.id)"
          style="border: 5px dotted blue"
        >
          <span>{{ searched.name }}</span>
          <el-button
            style="float: right; padding: 5px; color: red"
            class="el-icon-delete"
            @click="tryDelVisible = true"
            v-if="GLOBAL.hasLogin"
          ></el-button>
          <el-dialog title="确认删除？" :visible.sync="tryDelVisible">
            <div>
              <el-button type="primary" @click="deleteItem(searched.id)"
                >确认</el-button
              >
              <el-button @click="tryDelVisible = false">取消</el-button>
            </div>
          </el-dialog>
          <!--
          <el-button
            style="float: right; padding: 5px; color: blue"
            class="el-icon-edit"
            @click="editItem"
            v-if="GLOBAL.hasLogin"
          ></el-button>
          -->
        </el-card>
        <el-card class="box-card" v-if="toShowId === searched.id">
          <h4>到其它城市的距离</h4>
          <div class="distance-list">
            <ul
              class="infinite-list"
              v-infinite-scroll="load"
              style="overflow: auto"
            >
              <li
                v-for="item in toShowDistanceLsA"
                :key="item"
                class="infinite-list-item"
              >
                {{ item.cityA_name }} -- {{ item.cityB_name }}
                {{ item.distance }} km
              </li>
              <li
                v-for="item in toShowDistanceLsB"
                :key="item"
                class="infinite-list-item"
              >
                {{ item.cityA_name }} -- {{ item.cityB_name }}
                {{ item.distance }} km
              </li>
            </ul>
          </div>
        </el-card>
      </el-col>
      <el-col :span="6" v-for="item in cities" :key="item">
        <el-card class="box-card" @click.native="showDetail(item.id)">
          <span>{{ item.name }}</span>
          <el-button
            style="float: right; padding: 5px; color: red"
            class="el-icon-delete"
            @click="tryDelVisible = true"
            v-if="GLOBAL.hasLogin"
          ></el-button>
          <el-dialog title="确认删除？" :visible.sync="tryDelVisible">
            <div>
              <el-button type="primary" @click="deleteItem(item.id)"
                >确认</el-button
              >
              <el-button @click="tryDelVisible = false">取消</el-button>
            </div>
          </el-dialog>
          <!--
          <el-button
            style="float: right; padding: 5px; color: blue"
            class="el-icon-edit"
            @click="editItem"
            v-if="GLOBAL.hasLogin"
          ></el-button>
          -->
        </el-card>
        <el-card class="box-card" v-if="toShowId === item.id">
          <h4>到其它城市的距离</h4>
          <div class="distance-list">
            <ul
              class="infinite-list"
              v-infinite-scroll="load"
              style="overflow: auto"
            >
              <li
                v-for="item in toShowDistanceLsA"
                :key="item"
                class="infinite-list-item"
              >
                <el-row>
                  <el-col :span="10" style="color: brown"
                    >{{ item.cityA_name }} -- {{ item.cityB_name }}</el-col
                  >
                  <el-col :span="10" style="margin-left: 20px; color: orchid">
                    {{ item.distance }} km</el-col
                  >
                </el-row>
              </li>
              <li
                v-for="item in toShowDistanceLsB"
                :key="item"
                class="infinite-list-item"
              >
                <el-row>
                  <el-col :span="10" style="color: brown"
                    >{{ item.cityA_name }} -- {{ item.cityB_name }}</el-col
                  >
                  <el-col :span="10" style="margin-left: 20px; color: orchid">
                    {{ item.distance }} km</el-col
                  >
                </el-row>
              </li>
            </ul>
          </div>
        </el-card>
      </el-col>
    </el-row>
  </div>
</template>

<script>
export default {
  name: "aCity",
  data() {
    return {
      toShowId: "",
      toShowDistanceLsA: [],
      toShowDistanceLsB: [],
      tryDelVisible: false,
    };
  },
  props: {
    cities: {
      type: Array,
      required: true,
    },
    searched: {
      type: Object,
      required: false,
    },
  },
  methods: {
    showDetail(id) {
      if (this.toShowId === id) {
        this.toShowId = "";
      } else {
        this.toShowId = id;
        this.$axios({
          method: "get",
          url: "/transportation/distances/",
          headers: {
            "Content-Type": "application/json",
          },
          params: {
            asA: id,
          },
        }).then((res) => {
          this.toShowDistanceLsA = res.data;
        });
        this.$axios({
          method: "get",
          url: "/transportation/distances/",
          headers: {
            "Content-Type": "application/json",
          },
          params: {
            asB: id,
          },
        }).then((res) => {
          this.toShowDistanceLsB = res.data;
        });
      }
    },
    deleteItem(id) {
      this.$axios({
        method: "delete",
        url: `/transportation/cities/${id}/`,
        headers: {
          "Content-Type": "application/json",
          Authorization: `Bearer ${localStorage.getItem("admin_access")}`,
        },
      }).then(() => {
        this.$emit("cityDeleted");
      });
      this.tryDelVisible = false;
    },
    // editItem() {},
  },
};
</script>

<style scoped>
ul li {
  list-style-type: none;
}
ul {
  padding-inline-start: 20px;
  padding-inline-end: 20px;
}
.distance-list {
  max-height: 150px;
  overflow-y: auto;
}
</style>